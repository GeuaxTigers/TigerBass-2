# hardware/controller-board

KiCad project for the TigerBass 2 controller PCB: the microcontroller, the external ADC and its front end, the string drive, USB, power, and the connectors to everything else in the instrument.

**Status: schematic in progress.** The project is `Controller_PCB/`, made in **KiCad 10.0**. CI checks it with the same version.

## How the schematic is organised

One sheet per block, so several people can work at once without touching the same file.

```text
Controller_PCB.kicad_sch            root: sheet boxes and the wires between them, nothing else
├── mcu.kicad_sch                   page 2   STM32F439, decoupling, crystal, boot, SWD     2xx
├── power.kicad_sch                 page 3   battery, charging, regulators                 3xx
├── amplitude_detection.kicad_sch   page 4   pickup front end, external ADC               4xx
│   └── 1v65_ref.kicad_sch          page 5   1.65 V reference, inside Amplitude Detection 5xx
├── string_drive.kicad_sch          page 6   string outputs and fret inputs                6xx
├── usb_midi.kicad_sch              page 7   USB connector, DIN MIDI out                   7xx
└── connectors.kicad_sch            page 8   off-board headers                             8xx
```

The last column is the reference range for that sheet. `U201` is on the MCU sheet, `R412` on Amplitude Detection.

Power, String Drive, USB and MIDI, and Connectors are empty boxes waiting for their owners.

## Working on a sheet

1. **Claim the sheet** on its issue before you open it.
2. **Work only inside your sheet.** If you need a new connection to another block, that means a new hierarchical label on your sheet and a new pin on your sheet's box on the root. Make the root change in its own small pull request, or ask the person who owns the root, so two people never edit the root at once.
3. **Annotate only your sheet.** In Tools → Annotate Schematic, the project is already set to "First free after sheet number × 100". Choose **Current sheet only**, never "Entire schematic". New parts on the String Drive sheet then come out as R601, R602, and never clash with anyone else's.
4. **Run ERC**, save, close KiCad, and commit only your sheet's file. If the root or the `.kicad_pro` changed and you did not mean to change them, discard those changes.
5. **Layout waits.** One person owns `Controller_PCB.kicad_pcb` and runs Update PCB from Schematic after sheets merge. Nobody else edits the board file.

| File | Who edits it |
| --- | --- |
| Your block's sheet | You, after claiming it |
| `Controller_PCB.kicad_sch` (root) | One person at a time, in a small pull request of its own |
| `Controller_PCB.kicad_pro` | Avoid changing settings in a sheet pull request |
| `Controller_PCB.kicad_pcb` | The layout owner only |

## Before going further

These decisions shape the board and are still open. Each has a record in [`docs/decisions/`](../../docs/decisions/).

- [Shared-fret ghosting](../../docs/decisions/0001-shared-fret-ghosting.md)
- [External ADC](../../docs/decisions/0002-external-adc.md)
- [Amplitude front end](../../docs/decisions/0003-amplitude-front-end.md)
- [Microcontroller](../../docs/decisions/0004-microcontroller.md)
- [Battery and power tree](../../docs/decisions/0009-battery-and-power-tree.md)
- [Board partition and connectors](../../docs/decisions/0010-board-partition-and-connectors.md)

## Conventions

- Name nets for what they carry: `FRET_07`, `STRING_DRV_2`, `PICKUP_3_RAW`. Avoid net names that are only pin names.
- Prefer hierarchical labels and sheet pins over global labels for signals between blocks, so the root sheet shows how the blocks connect.
- Add a test point for every power rail and for each signal named in the bring-up checklist.
- Board revisions are letters: `revA`, `revB`. Put the revision in the title block and on the silkscreen.
- Keep file names lower case with underscores. Spaces in file names work in KiCad but are awkward on the command line.

## Who edits here

The PCB team. Check the issue for the sheet you want to change before you open it.
