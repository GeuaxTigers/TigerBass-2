# Architecture

How TigerBass 2 is put together: the blocks, where each one lives, and what crosses between them.

**This describes the plan, not a finished design.** The blocks follow from the project goals and from how Tiger Bass 1 worked. The values in the tables are mostly `TBD(team)` because the decisions that set them are still open. Update this file whenever a decision in [`decisions/`](decisions/) is settled.

## The instrument

```mermaid
flowchart LR
    STR["Strings x4"]
    HOST["Computer or synth"]

    subgraph NECK["Neck"]
        FB["Fret board<br/>20 fret lines"]
    end

    subgraph BODY["Body"]
        PU["Per-string pickups<br/>4 channels"]
        EXP["Scroll wheel<br/>Whammy bar"]
        BAT["Battery<br/>charging and protection"]

        subgraph CB["Controller board"]
            DRV["String drive<br/>4 outputs"]
            FIN["Fret inputs<br/>20 lines"]
            AFE["Pickup front end<br/>4 channels"]
            ADC["External ADC"]
            MCU["Microcontroller"]
            USB["USB"]
            DIN["DIN MIDI out"]
            PWR["Regulation"]
        end
    end

    MCU --> DRV
    DRV -->|"one string driven at a time"| STR
    STR -.->|"pressed onto a fret"| FB
    FB --> FIN
    FIN --> MCU
    STR -.->|"vibration"| PU
    PU --> AFE
    AFE --> ADC
    ADC --> MCU
    EXP --> MCU
    MCU --> USB
    USB --> HOST
    MCU --> DIN
    DIN --> HOST
    BAT --> PWR
    PWR --> MCU
```

Dotted arrows are physical, not electrical: a string touching a fret, a string vibrating over a pickup.

Not shown: the bass's normal audio path, from its magnetic pickup to the output jack. Whether that path touches the controller board is an open question in [`requirements.md`](requirements.md#open-questions-about-the-requirements-themselves).

## What each block does

| Block | Job | Tiger Bass 1 | TigerBass 2 |
| --- | --- | --- | --- |
| String drive | Puts a voltage on one string at a time | 4 GPIO, each through a Schottky diode | Same principle. See [0001](decisions/0001-shared-fret-ghosting.md) |
| Fret board | Carries each fret's signal out of the neck | 80 lines, one per string segment of each fret | 20 lines, one per full-width fret |
| Fret inputs | Reads which fret lines are high while a string is driven | 80 GPIO, 1 kΩ series each, internal pull-downs | 20 lines. Protection and pull-down values `TBD(team)` |
| Pickups | One signal per string that rises when that string is plucked | 4 channels. The report and the as-built board disagree on type, see the legacy notes | `TBD(team)` |
| Pickup front end | Conditions the pickup signal for the ADC | Passive: series resistor, clamp diodes, series diode | See [0003](decisions/0003-amplitude-front-end.md) |
| ADC | Digitises the four pickup channels | Microcontroller's internal 12-bit ADC | External part. See [0002](decisions/0002-external-adc.md) |
| Microcontroller | Runs the scan, decides notes and velocities, builds MIDI | STM32F439ZIT6 | See [0004](decisions/0004-microcontroller.md) |
| Expression | Player controls beyond the strings | 5 potentiometers | Scroll wheel and whammy bar. See [0008](decisions/0008-expression-sensing.md) |
| USB | MIDI to a computer | Full-speed, Micro-B | See [0006](decisions/0006-usb.md) |
| DIN MIDI out | MIDI to hardware synths | UART through a buffer | See [0007](decisions/0007-din-midi-out.md) |
| Battery and regulation | Powers everything | 9 V rechargeable Li-ion, LDO to 3.3 V | See [0009](decisions/0009-battery-and-power-tree.md) |

## One note, start to finish

```mermaid
sequenceDiagram
    participant P as Player
    participant PU as Pickup and ADC
    participant FW as Firmware
    participant M as String and fret matrix
    participant H as Host

    P->>PU: plucks string 2
    PU->>FW: channel 2 rises above threshold
    FW->>M: drive string 2 high
    M->>FW: fret lines that read high
    FW->>FW: fret closest to the bridge wins, or open string
    FW->>FW: track the peak for velocity
    FW->>H: Note On, note and velocity
    Note over P,H: string rings
    PU->>FW: channel 2 falls below release threshold
    FW->>H: Note Off, same note
```

This is the sequence Tiger Bass 1 used, taken from its system flowchart. Two things in it are design choices the firmware can change without new hardware:

- **Whether to wait for the peak before sending Note On.** Waiting gives a better velocity and costs latency. Sending at the threshold crossing is faster and has to guess the velocity.
- **Whether to scan the frets only after a pluck, or continuously.** Scanning continuously means the fretted position is already known when the pluck arrives, and lets a hammer-on be detected without a pluck.

Each stage's contribution to latency is the subject of Prompt 6 in [`copilot/kickoff-prompt.md`](copilot/kickoff-prompt.md).

## Interfaces that cross a board boundary

Everything that leaves the controller board through a connector. This table is what the fret board designer, the mechanical engineers and the controller board designer have to agree on.

| Signal | Between | Count | Direction, seen from the controller | Level | Connector | State |
| --- | --- | --- | --- | --- | --- | --- |
| Fret lines | Fret board and controller | 20 | In | Follows the string drive voltage. 3.3 V on Tiger Bass 1 | `TBD(team)`. Was two 2×20 headers | Count depends on [0001](decisions/0001-shared-fret-ghosting.md) |
| Fret board ground or return | Fret board and controller | `TBD(team)` | n/a | 0 V | Same connector | Open |
| String drive | Controller and each string | 4 | Out | 3.3 V through a diode on Tiger Bass 1 | `TBD(team)`. Was a 1×4 header | Where the wire attaches to each string is `TBD(team)` |
| Pickup signal | Pickups and controller | 4 | In | `TBD(team)`. Needs measuring on the real pickup | `TBD(team)`. Was a 2×4 shrouded header with a ground beside each signal | Open |
| Pickup power | Controller and pickups | `TBD(team)` | Out | `TBD(team)`. Tiger Bass 1 had a 9 V output header | `TBD(team)` | Depends on the pickup |
| Scroll wheel | Sensor and controller | `TBD(team)` | In | `TBD(team)` | `TBD(team)` | [0008](decisions/0008-expression-sensing.md) |
| Whammy bar | Sensor and controller | `TBD(team)` | In | `TBD(team)` | `TBD(team)` | [0008](decisions/0008-expression-sensing.md) |
| Other controls | Body-mounted controls and controller | `TBD(team)` | In | `TBD(team)` | `TBD(team)` | Which of octave, MIDI channel and threshold are kept is open |
| USB | Controller and host | D+, D−, VBUS, ground, shield | Both | USB full-speed on Tiger Bass 1 | `TBD(team)`. Was Micro-B | [0006](decisions/0006-usb.md) |
| DIN MIDI out | Controller and synth | 1 current loop | Out | 3.3 V MIDI on Tiger Bass 1 | `TBD(team)` | [0007](decisions/0007-din-midi-out.md) |
| Battery | Battery and controller | 2, plus any sense lines | In | `TBD(team)`. Was 9 V | `TBD(team)` | [0009](decisions/0009-battery-and-power-tree.md) |
| Charging | Outside and battery | `TBD(team)` | In | `TBD(team)` | `TBD(team)` | [0009](decisions/0009-battery-and-power-tree.md) |
| Debug | Controller and programmer | SWDIO, SWCLK, SWO, reset, 3.3 V, ground | Both | 3.3 V | 2×5, 1.27 mm on Tiger Bass 1 | Keep unless the MCU changes family |

Questions this table raises for the mechanical side: which of these connectors must be reachable with the instrument assembled (USB, charging, MIDI out), which only during assembly (fret board, pickups, battery), and which only on the bench (debug).

## Firmware structure

Planned, not implemented. The reasons for the split are in [`firmware/README.md`](../firmware/README.md).

```mermaid
flowchart TB
    subgraph APP["firmware/app: portable logic, tested on a PC"]
        VOICE["voice<br/>one state machine per string"]
        PLUCK["pluck_detect"]
        SCAN["fret_scan"]
        VEL["velocity"]
        NOTE["note_map"]
        EXPR["expression"]
        MSG["midi_msg"]
        CFG["config"]
    end

    subgraph HAL["Interfaces declared in app, implemented in bsp"]
        H1["drive a string"]
        H2["read fret lines"]
        H3["read pickup sample"]
        H4["read expression control"]
        H5["time in microseconds"]
        H6["queue MIDI message"]
    end

    subgraph BSP["firmware/bsp: STM32 specific"]
        GPIO["GPIO"]
        ADCD["External ADC driver"]
        TIM["Timer"]
        USBD["USB MIDI class"]
        UART["UART for DIN MIDI"]
    end

    VOICE --> PLUCK
    VOICE --> SCAN
    VOICE --> VEL
    VOICE --> NOTE
    VOICE --> MSG
    EXPR --> MSG
    SCAN --> H1
    SCAN --> H2
    PLUCK --> H3
    PLUCK --> H5
    EXPR --> H4
    MSG --> H6
    H1 --> GPIO
    H2 --> GPIO
    H3 --> ADCD
    H4 --> ADCD
    H5 --> TIM
    H6 --> USBD
    H6 --> UART
```

`H4` is drawn going to the ADC driver, but whether the expression controls share the external ADC, use the microcontroller's own ADC, or are digital (an encoder) is part of [0008](decisions/0008-expression-sensing.md).

## Keeping this file current

When a decision is recorded in [`decisions/`](decisions/), update the matching rows here in the same pull request. A `TBD(team)` that has an answer somewhere else in the repo is worse than one that has no answer, because it tells the reader nobody knows.
