# Instructions for Copilot in this repository

## What this project is

TigerBass 2 is a senior-design capstone at LSU: a 4-string bass guitar that also works as a MIDI controller. A team of eight students (six electrical and computer engineers, two mechanical engineers) is redesigning the controller electronics from last year's "Tiger Bass 1". Read `README.md` for the goals and `docs/legacy/tigerbass1-notes.md` for what is known about the old board.

The team designs the hardware in KiCad, writes firmware in C for an STM32, and tests on a real instrument. You work in the background on the parts of the project that can be done and checked without hardware: repository structure, documentation, analysis, scripts, continuous integration, and firmware logic that can be unit-tested on a PC.

Most of the team is new to Git. Several have never reviewed a pull request. Write for them: explain what a change does and how to check it, and keep each pull request small enough to read in one sitting.

## What you can and cannot know

You cannot open KiCad's editors, flash a board, probe a signal or hear the instrument. The students can. This shapes how you work.

- **Do not make design decisions.** Part choices, pin assignments, the latency target, the power architecture and everything else in the README's "Open decisions" list belong to the team. When a task touches one, lay out the options, the criteria and what each option costs, then stop. Write code against an interface so it works whichever way the decision goes.
- **Do not invent facts.** No made-up part numbers, pin numbers, register values, measurements, prices or stock levels. If you need a value you do not have, write `TBD(team)` and list it in the pull request description under "Questions for the team". A visible gap is useful to them. A plausible invented number can end up on a circuit board.
- **Say where a fact came from.** For a claim about the old design, cite the file and section in this repo. For a claim about a component, link the manufacturer's datasheet and give the page or table. If you could not open the source, say that instead of citing it.
- **Treat last year's documents as evidence, not truth.** `docs/legacy/tigerbass1-notes.md` lists places where they contradict each other. When you find another, add it to that list.

## Files you must not change

- `legacy/**`. This is last year's work, kept as a read-only reference. Read it, quote it, never edit it.
- `*.kicad_sch`, `*.kicad_pcb`, `*.kicad_pro`, `*.kicad_sym`, `*.kicad_mod`. These are written by KiCad's editors. Editing them as text can produce a file that opens but is electrically wrong, and nobody would see it in a diff. You may run `kicad-cli` on them to check rules and export netlists, BOMs and PDFs.
- `firmware/bsp/**` outside the `USER CODE BEGIN` / `USER CODE END` markers. STM32CubeMX regenerates everything else in those files and would erase your edit.
- Licence files. Do not add or choose a licence.

If a task seems to need one of these changed, describe the change in the pull request and leave it for a student to make.

## Where things go

| Path | Contents |
| --- | --- |
| `hardware/controller-board/`, `hardware/fret-board/` | KiCad projects, edited by students |
| `hardware/lib/` | Project symbols, footprints, 3D models |
| `hardware/sim/` | LTspice and other circuit simulations |
| `firmware/app/` | Portable C. No vendor headers. Must compile on a PC |
| `firmware/bsp/` | STM32 HAL glue and CubeMX output. Implements the interfaces `app/` declares |
| `firmware/tests/` | Unit tests that run on a PC |
| `tools/` | Python scripts. Standard library only unless a `requirements.txt` says otherwise |
| `docs/decisions/` | One file per design decision |
| `docs/test/` | Test plans, procedures, results |
| `docs/copilot/` | The prompts the team gives you |

## Firmware rules

The firmware has one hard job: turn a pluck into a MIDI message in a few milliseconds, every time. That is why the rules below exist.

- **Keep logic and hardware apart.** Everything that decides something (which note, what velocity, when to send Note Off) lives in `firmware/app/` and reaches hardware only through small interfaces declared there, for example reading the fret lines, driving a string, reading a pickup sample, getting the time in microseconds, queueing a MIDI message. This is what lets the logic be tested on a PC, and it means a change of microcontroller or ADC touches `bsp/` only.
- **Every `app/` module has tests** that run with `ctest` on a PC, using fake implementations of those interfaces. Test the awkward cases: two strings at once, a note changing while the string is still ringing, a fret line that bounces, a pickup signal sitting right at the threshold.
- **Nothing in the scan path waits.** No delay calls, no loop that waits on a flag without a timeout, no blocking USB send. Last year's loop waited for USB before every packet. Queue the message and return.
- **No dynamic memory.** No `malloc`, no variable-length arrays. Sizes are fixed at compile time.
- **Derive, do not hand-type.** The note for a string and fret is the open-string note plus the fret number. Generate tables from the tuning and the fret count, so a change to either is one edit.
- **C11, fixed-width integer types, warnings as errors** (`-Wall -Wextra -Werror`) for everything in `app/` and `tests/`.
- **Name the unit.** `threshold_mv`, `window_us`, `velocity_midi`. A bare `threshold` gets misread.

## Domain facts you can rely on

These are standards, not project decisions.

- Standard 4-string bass tuning: E1, A1, D2, G2, which are MIDI notes 28, 33, 38, 43 (with middle C, C4, as 60).
- A fretted note is the open-string note plus the fret number in semitones. Fret 0 is the open string.
- MIDI channel voice messages: Note On `0x9n note velocity`, Note Off `0x8n note velocity`, Control Change `0xBn controller value`, Pitch Bend `0xEn lsb msb`. `n` is the channel, 0 to 15. Data bytes are 0 to 127.
- Note On with velocity 0 means Note Off. A real Note On needs a velocity of 1 to 127.
- Pitch bend is 14 bits, 0 to 16383, with 8192 as centre. The least significant 7 bits are sent first.
- The modulation wheel is Control Change 1.
- A USB-MIDI 1.0 event packet is 4 bytes: `(cable << 4) | code`, then the 3 MIDI bytes. The code for a channel voice message is the high nibble of its status byte: 0x9 for Note On, 0x8 for Note Off, 0xB for Control Change, 0xE for Pitch Bend.
- DIN MIDI is a serial stream at 31250 baud, 8 data bits, no parity, 1 stop bit.

Project values that look like facts but are not settled: the number of frets wired for sensing (20 is the current plan), whether the legacy note table's fret numbering is right (see the legacy notes), the latency target, and every part number.

## Checking your work

Run these before you open a pull request, and paste the result into its description. If a command does not work yet, the build it belongs to has not been created. Setting it up is one of the early tasks in `docs/copilot/kickoff-prompt.md`.

```bash
# Firmware logic and its tests, on a PC
cmake -S firmware -B build/host -DTB2_BUILD_TESTS=ON
cmake --build build/host
ctest --test-dir build/host --output-on-failure

# Hardware rule checks, only when a KiCad project exists under hardware/
kicad-cli sch erc --exit-code-violations <path/to/project.kicad_sch>
kicad-cli pcb drc --exit-code-violations <path/to/project.kicad_pcb>
```

Do not disable or delete a failing test to get a green run. If a test is wrong, say why in the pull request and fix the test.

## Pull requests

- One task, one pull request. If a task grows, finish the first coherent piece and list the rest as follow-ups.
- Title: an area prefix and a plain sentence. `fw: derive note map from tuning and fret count`. Prefixes: `fw`, `hw`, `mech`, `docs`, `tools`, `ci`.
- Description, in this order:
  1. **What changed**, in two or three sentences a teammate can read without opening the diff.
  2. **Why.**
  3. **How it was checked**, with the commands and their output.
  4. **Assumptions**, each one something a student should confirm.
  5. **Questions for the team**, including every `TBD(team)` you added.
  6. **Not done**, anything in the task you left out and why.
- Documentation is Markdown, with diagrams as Mermaid code blocks so they render on GitHub and can be reviewed as text.
