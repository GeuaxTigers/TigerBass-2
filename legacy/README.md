# legacy

Last year's work, kept as a read-only reference.

## `tigerbass1/`

The 2024–25 Project 68 controller board and firmware. **Nothing has been imported yet.**

What to put there:

| What | Notes |
| --- | --- |
| The `ControllerPCB` KiCad project | Project file, all four schematic sheets, the board file, and any project-local symbol and footprint libraries |
| The fret board KiCad project | If it is a separate project |
| The STM32CubeIDE firmware project | Including `.project`, `.cproject` and the `.ioc` file |
| LTspice simulations | The `.asc` files |
| The Final Design Project Report | As a PDF |
| Gerbers and the BOM that were actually ordered | So we know which revision was built |

What to leave out: `#auto_saved_files#`, `*-backups` folders, backup zips, `*.lck` files, and CubeIDE `Debug/` and `Release/` build folders. `.gitignore` already excludes most of these.

Suggested layout:

```text
legacy/tigerbass1/
├── hardware/controller-board/
├── hardware/fret-board/
├── firmware/
├── sim/
└── docs/
```

## Rules

- **Do not edit anything under `legacy/`.** It records what was built. If you want to reuse a circuit, copy it into the new project and change it there.
- **Do not open last year's KiCad files with a newer KiCad and save.** That rewrites them in the new format. Open them read-only, or work on a copy outside the repo.
- What we have worked out about the old design so far is in [`docs/legacy/tigerbass1-notes.md`](../docs/legacy/tigerbass1-notes.md). It was written from the design report and screenshots, before these files were available. Once they are here, Prompt 2 in [`docs/copilot/kickoff-prompt.md`](../docs/copilot/kickoff-prompt.md) checks those notes against the real files.
