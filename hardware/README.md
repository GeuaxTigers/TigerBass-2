# hardware

The electronics design: schematics, board layouts, the parts libraries they use, and the simulations that justify them.

| Folder | Contents |
| --- | --- |
| [`controller-board/`](controller-board/) | KiCad project for the new controller PCB |
| [`fret-board/`](fret-board/) | KiCad project for the neck / fret PCB |
| [`lib/`](lib/) | Symbols, footprints and 3D models shared by both projects |
| [`sim/`](sim/) | LTspice and other circuit simulations |

## Rules for this folder

- **One person edits a given KiCad file at a time.** Claim it on the issue first. See [`CONTRIBUTING.md`](../CONTRIBUTING.md#kicad-files).
- **Everyone uses the same KiCad major version.** The version is pinned at the top of [`.github/workflows/ci.yml`](../.github/workflows/ci.yml) and listed in the root README.
- **Run ERC and DRC before you open a pull request.** CI runs them again and fails the pull request on errors.
- **Do not commit gerbers, drill files or other fabrication output.** They are generated from a tagged commit and attached to a GitHub release, so an output file can never disagree with the board file it came from.
- **Every part has a manufacturer part number in its symbol fields.** A BOM that says "10k resistor" cannot be ordered.

## What does not belong here

Firmware (`firmware/`), mechanical CAD (`mechanical/`), last year's board (`legacy/tigerbass1/`), and datasheets. Link to the manufacturer's datasheet from the symbol's Datasheet field instead of committing the PDF.
