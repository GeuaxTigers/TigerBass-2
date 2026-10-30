# Tiger Bass 1: what we know about the old board

Notes on the 2024–25 controller board (Project 68), written in October 2026 from last year's Final Design Project Report (FDPR) and from screenshots and photos of the KiCad project, the firmware and the finished instrument.

**None of this was read from the KiCad or firmware source files.** Those are not in this repo yet. When they are imported into `legacy/tigerbass1/`, check every table here against them and correct this file. Where a screenshot and the FDPR disagree, the screenshot is the later source and is more likely to match what was built.

How sure each item is:

- **Seen**: read directly off a schematic screenshot, board render or code screenshot.
- **Report**: stated in the FDPR, which was written before the board was designed.
- **Inferred**: worked out from the above. Verify before relying on it.

## The system in one paragraph

A 4-string bass with a neck PCB under the frets. Each fret is split into four segments, one per string, and each segment is wired to its own microcontroller input. The four strings are wired to four microcontroller outputs. The firmware watches four pickup channels on the ADC. When a string's channel goes above a threshold, the firmware drives that string high, reads that string's fret inputs, and sends MIDI Note On for the fret closest to the bridge, or for the open string if no fret is high. When the channel falls below the threshold it sends Note Off for that string. MIDI goes out over USB. (Seen: system flowchart.)

## KiCad project

| Item | Value | Source |
| --- | --- | --- |
| Project name | `ControllerPCB` | Seen |
| KiCad version | 8.0.8 | Seen, title block |
| Root sheet | `ControllerPCB.kicad_sch`, sheet 1 of 4 | Seen |
| Sub-sheets | `Power.kicad_sch`, `midi_out.kicad_sch`, `amp_detection.kicad_sch` | Seen |
| Board | Roughly square, 4 mounting holes, purple solder mask, tiger silkscreen | Seen |

Last year's Git history includes KiCad autosave files (`#auto_saved_files#`), backup zips and a lock file (`~ControllerPCB.kicad_sch.lck`). (Seen: a commit screenshot.) Leave them out when importing.

## Microcontroller and support circuitry

| Item | Value | Source |
| --- | --- | --- |
| MCU | STM32F439ZIT6, LQFP-144 | Seen and Report |
| Crystal | 8 MHz on PH0 / PH1 | Seen |
| Boot | BOOT0 on an SPDT switch | Seen |
| Analog supply | VDDA fed through a 120 Ω ferrite bead with its own decoupling | Seen |
| Core regulator caps | 2.2 µF on VCAP_1 and VCAP_2 | Seen |
| Debug | 10-pin SWD header: SWDIO PA13, SWCLK PA14, SWO PB3, NRST | Seen |
| LEDs | 3V3 power LED, one debug LED | Seen |
| Prototyping board | NUCLEO-F439ZI | Report |

## Pin map

From a schematic screenshot dated 25 February 2025. The board was not ordered until later, so pins may have moved.

| Pin | Net | Function |
| --- | --- | --- |
| PA0 – PA3 | `ADC_String_1` … `ADC_String_4` | Pickup channel per string, ADC1_IN0 – IN3 |
| PA4 | `ADC_Octave` | Octave control pot |
| PA5 | `ADC_Mod` | Modulation pot |
| PA6 | `ADC_Bend` | Pitch bend pot |
| PA7 | `ADC_MIDI_Ch` | MIDI channel selector pot |
| PB0 | `ADC_Thresh` | Threshold pot |
| PA9 | `VBUS_SENSE` | USB VBUS through a 33 kΩ / 82 kΩ divider |
| PA11, PA12 | `D-`, `D+` | USB OTG_FS |
| PA13, PA14, PB3 | SWDIO, SWCLK, SWO | Debug |
| PH0, PH1 | HSE | Crystal |
| A UART TX pin, not identified | `UART_TX` | DIN MIDI out |
| 4 GPIO, not identified | String outputs | One per string |

Fret inputs, 80 in total, each through a 1 kΩ series resistor, on two 2×20 headers:

| Header | Odd pins (1 – 39) | Even pins (2 – 40) |
| --- | --- | --- |
| Conn_1 | PA8, PA10, PA15, PB10 – PB15, PC0 – PC10 | PC11 – PC15, PD0 – PD6, PD8 – PD15 |
| Conn_2 | PE2 – PE15, PF0 – PF5 | PF6 – PF15, PG0 – PG9 |

Which fret and string each pin maps to is not known from the screenshots.

## Connectors

| Silkscreen | Connector | Purpose | Source |
| --- | --- | --- | --- |
| USB | Micro-B, Digi-Key 732-5960-1-ND | MIDI over USB. TVS diodes on VBUS, D+ and D−. 90 Ω differential pair | Seen |
| SWD | 2×5, 1.27 mm (FTSH-105 family) | Programming and debug | Seen |
| STRING OUT | 1×4 | Drive lines to the four strings, one series diode each | Seen |
| PICKUP IN | 2×4 shrouded | Four pickup channels, with ground on the opposite row | Seen |
| Neck PCB inputs | 2 × 2×20 shrouded, Amphenol 75869-107LF | 80 fret-segment lines | Seen |
| MIDI CONTROL INPUTS | 5 × 3-pin | 3.3 V, wiper, ground for each control pot | Seen |
| MIDI OUT | 3-pin | DIN MIDI out | Seen |
| 9V OUT | 2-pin | 9 V to off-board electronics | Seen |
| Battery | 2-pin power connector | 9 V in | Seen |

## Circuits

### Note detection

- Strings are outputs, fret segments are inputs. One string is driven high at a time. (Seen and Report.)
- The MCU inputs use the internal pull-downs, taken as 40 kΩ. (Report.)
- Each string output has a series Schottky diode so a driven string cannot push current into another string's low output. The report selected the ROHM RB886CM. (Report. Four diodes are visible by STRING OUT on the board.)
- Pressing a string was modelled as contacting two frets, the one on each side of the finger. The firmware takes the high fret closest to the bridge. (Report.)
- Simulated levels with a 3.3 V drive: 3.15 V at the target input in the typical case, 2.95 V with every fret contacted, against a 2.3 V logic-high threshold. Worst-case source current 1.77 mA against an 8 mA limit. (Report, LTspice.)
- Finger resistance measured across two strings: 1759 – 1995 kΩ dry, 201 – 553 kΩ wet with salt water. (Report, bench test.)

### Amplitude detection

As built, per string (Seen: `amp_detection` sheet, 19 March 2025):

- 1 kΩ series resistor from the pickup input.
- A clamp diode to ground, and a clamp diode to a node set to about 3 V by a 220 Ω / 2 kΩ divider from 3.3 V.
- A series diode to the ADC pin (`ADC_String_n_F`).

The FDPR describes a different design: a Cycfi Nu Multi 6 hex magnetic pickup into an op-amp precision rectifier (two OP07 and two MBR0520L per string). The spring 2025 system flowchart and budget show piezo pickups instead (a Graphtec Ghost pickup at $255, alongside an EMG 35DC at $255). The op-amp rectifier does not appear on the board. (Inferred from the board render: 12 diodes by PICKUP IN, no op-amps.)

### MIDI over USB

- Full-speed USB device on PA11 / PA12. (Seen.)
- The firmware sends 4-byte USB-MIDI event packets by calling `USBD_HID_SendReport`, after a busy-wait on the HID state. (Seen: code screenshots.) So the stack started from ST's HID class.
- Demo code packets: Note On `{0x09, 0x90, note, velocity}`, Note Off `{0x08, 0x80, note, velocity}`. (Seen.)

### DIN MIDI out

UART TX → SN74LVC1G17 Schmitt buffer → 10 Ω to the data pin, with 33 Ω from 3.3 V to the source pin. (Seen.)

### Power

- Pale Blue 9 V rechargeable Li-ion battery, charged over its own USB-C port. (Report.)
- Analog Devices LT1763 LDO to 3.3 V. (Report. An SO-8 regulator is visible on the board.)
- Estimated MCU current: 67 mA at 25 °C, 107 mA at 105 °C. Estimated run time about 3 hours. (Report.)

## Firmware

- STM32CubeIDE, ST HAL, a single `main.c` super-loop. (Seen.)
- Note lookup is an 80-entry table, `notes[80]`, 20 entries per string, indexed from fret 20 down to fret 1. (Seen: `MIDI NOTE TABLE.txt`.)
- Lowest entry per string: 0x1C, 0x21, 0x26, 0x2B, which are E1, A1, D2, G2, standard bass tuning.

## Performance figures

| Quantity | Value | Source |
| --- | --- | --- |
| Latency target, pluck to MIDI packet | 8 ms | Report |
| Predicted worst case | 4.84 ms | Report, analysis |
| Time for the pickup signal to reach its first peak, lowest note | 6 ms simulated at 41 Hz; 2.5 ms from a recorded waveform | Report |
| Budget spent, spring 2025 | Pickups $255 + $255, PCBs $225.30, fret board $62.39, electrical materials $40 | Seen, budget chart |

## Things that contradict each other

Check each of these against the real files before reusing anything.

1. **Note table numbering.** The entry labelled "Fret 1" on each string is the open-string pitch (E1, A1, D2, G2). On a real bass, fret 1 is a semitone above the open string. Either the table is a semitone flat, or "fret 1" means something else on this instrument. The firmware also sends a separate "open note" when no fret is high.
2. **Note table comments.** Entries commented with a `b` suffix are sharps. For example 0x1E is commented `F1b` and is F♯1.
3. **Packet headers in the note table file.** It declares `midi_msg_on = {0x08, 0x08, 0, 0}` and `midi_msg_off = {0x09, 0x09, 0, 0}`. In USB-MIDI, 0x09 / 0x9n is Note On and 0x08 / 0x8n is Note Off, which is what the earlier demo code uses. The file's values look swapped and have the wrong status bytes.
4. **USB speed.** The FDPR's timing analysis assumes high-speed USB at 480 Mbit/s and gets 6.25 ns to send a packet. The board uses the full-speed port. On full-speed USB, transfers are scheduled in 1 ms frames, so USB can add up to about 1 ms, before any delay in the host.
5. **ADC timing.** The FDPR takes 0.5 µs per conversion and plans 9400 back-to-back samples over 4.7 ms. That is a best case for one channel. Four string channels, five control channels and a 1 kΩ-plus-diode source impedance all slow it down.
6. **Pickup and rectifier.** Hex magnetic pickup and op-amp rectifier in the report, piezo channels and a passive diode network on the board. See "Amplitude detection" above.
7. **Controls.** The March 2025 schematic shows six 3-pin control headers. The finished board has five.

## Things to think about for TigerBass 2

- The series diode in front of the ADC costs a diode drop, so quiet notes do not register. That sets a floor on the threshold.
- The firmware blocks: it busy-waits for USB before every send. A scan loop that must be fast and regular should not wait on USB.
- 80 lines between neck and body needed two 40-pin ribbon cables. Twenty shared frets cut that to 20 lines, at the cost of the ghosting question in the README.
