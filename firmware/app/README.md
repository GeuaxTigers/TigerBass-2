# firmware/app

The hardware-independent firmware: everything that makes a decision.

**Status: not started.**

## Planned modules

| Module | Job |
| --- | --- |
| `config` | Fret count, string count, tuning, default MIDI channel, and every tunable number, in one place |
| `note_map` | MIDI note for a string and fret, computed from the tuning |
| `midi_msg` | Build Note On, Note Off, Control Change and Pitch Bend, as MIDI bytes and as USB-MIDI packets |
| `hal` | Headers only: the interfaces `bsp/` must implement |
| `fret_scan` | From the fret lines read while one string is driven, find the fretted position, with debounce |
| `pluck_detect` | Per string: detect a pluck, track its peak, detect release |
| `velocity` | Map a peak level to MIDI velocity |
| `voice` | One state machine per string tying the above together |
| `expression` | Turn the scroll wheel and whammy bar readings into Control Change and Pitch Bend |

## What may be included here

The C standard library headers and other headers in this folder. Nothing else. If a file here includes `stm32f4xx_hal.h` or any other vendor header, it stops compiling on a PC and its tests stop running.

## What does not belong here

Pin numbers, register writes, peripheral setup, interrupt handlers and anything else specific to one microcontroller. That is [`bsp/`](../bsp/).

## Who edits here

Whoever is writing firmware logic. Unlike KiCad files, C source merges well, so several people can work here at once on separate modules.
