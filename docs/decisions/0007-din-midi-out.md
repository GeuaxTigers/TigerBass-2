# 0007: DIN MIDI out

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)` |
| Needed by | Before the controller schematic's connector sheet |
| Requirements | F4 |
| Analysis | None planned |

## The question

Does TigerBass 2 have a traditional MIDI output for hardware synthesizers, in addition to USB?

## Why it matters

A serial MIDI output lets the instrument drive a hardware synth directly, with no computer. It costs a UART pin, a buffer, two resistors, a connector and a hole in the body. It also has its own latency: MIDI serial runs at 31250 baud, so a three-byte Note On takes about 1 ms to send.

## What Tiger Bass 1 did

A UART transmit pin through an SN74LVC1G17 buffer, with 10 Ω in the data line and 33 Ω from 3.3 V to the source pin, out to a 3-pin header. Whether a MIDI jack was ever wired to that header on the finished instrument is not known from our notes.

## Options

### A. Drop it

USB only. Fewer parts, one less cutout. The instrument needs a computer, or a USB MIDI host, to make sound.

### B. Keep it on a 5-pin DIN jack

The connector every hardware synth has. The jack is large, which matters on a sleek body.

### C. Keep it on a 3.5 mm TRS jack

The MIDI specification also defines MIDI over a 3.5 mm TRS jack. Much smaller cutout. The user needs a TRS-to-DIN adapter cable for most synths.

### D. Populate the circuit, decide on the jack later

Keep the buffer and header on the board, and leave the jack as a mechanical decision. Costs a pin and a few small parts, and keeps the option open.

## How to choose

1. Whether the sponsor wants to use the instrument without a computer.
2. Room on the body for the jack (TB2-05).
3. Whether a spare UART is available on the chosen microcontroller ([0004](0004-microcontroller.md)).
4. Usefulness on the bench: a serial MIDI line is easy to put on a scope, which makes it a convenient "MIDI sent" marker for latency tests ([0005](0005-latency-target.md)).

## What we need to find out first

1. **Ask the sponsor** whether hardware synths are part of how the instrument will be used.
2. **Check whether Tiger Bass 1's MIDI out was ever used.**
3. **Ask the mechanical engineers** what each jack costs them in the body design.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
