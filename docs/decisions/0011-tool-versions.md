# 0011: KiCad and CubeIDE versions

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)` |
| Needed by | Before anyone creates a KiCad or CubeIDE project in this repo |
| Requirements | None. This is a working agreement |
| Analysis | None needed |

## The question

Which version of KiCad and which version of STM32CubeIDE does everyone on the team install?

## Why it matters

A newer KiCad saves schematic and board files in a format an older KiCad cannot open. If one person upgrades and saves, everyone else is locked out of that file until they upgrade too. CubeIDE and CubeMX behave similarly: opening a project in a newer version migrates it, and regenerating code with a different firmware package version changes generated files across the project.

This costs nothing to decide and blocks everyone until it is decided.

## KiCad

Last year's project was made in KiCad 8.0.8.

### A. KiCad 8, the latest 8.0.x

Opens last year's files without converting them, so circuits can be copied across directly.

### B. The current stable KiCad release

Newer features and fixes. Opening last year's files in it converts them, so work on a copy and leave the originals in `legacy/` untouched. Check that the lab computers have it or can get it.

## STM32CubeIDE

`TBD(team)`. Find out which version last year's firmware project was made with, by opening its `.ioc` or `.project` file in a text editor, and which STM32Cube firmware package version it used.

### A. The version last year's project used

Opens the old project unchanged.

### B. The current release

Start the new project fresh in it. Last year's project is then a reference to read, not a base to build on.

## How to choose

1. What the lab computers have installed, and whether students can update them.
2. Whether we expect to copy sheets from last year's KiCad project directly.
3. Every team member can install it on their own computer and operating system.

## What we need to find out first

1. **What is installed on the lab computers.**
2. **Each team member's operating system**, to confirm the chosen versions are available for all of them.
3. **The CubeIDE and firmware package versions** in last year's project.

## Once decided

- Write the versions in the README's "Getting set up" table.
- Set `KICAD_VERSION` at the top of [`.github/workflows/ci.yml`](../../.github/workflows/ci.yml) to match, so CI checks files with the same KiCad the team draws them in.
- Record the STM32Cube firmware package version in `firmware/bsp/README.md`.
- Agree the rule for upgrading: a team decision, everyone on the same day, in its own pull request that does nothing else.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
