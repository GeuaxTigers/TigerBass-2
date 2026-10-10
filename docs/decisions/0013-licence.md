# 0013: Licence

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)`, with the sponsor and advisor |
| Needed by | Before the repository is made public, or shared outside the team |
| Requirements | None |
| Analysis | None needed |

## The question

Under what terms, if any, may people outside the team use what is in this repository?

## Why it matters

The repository is private and has no licence file. With no licence, nobody outside has permission to copy or reuse anything in it, which is the right default for now.

It becomes a real question if the team wants to put the project in a portfolio, publish it, or hand it to next year's team in a public repository. It is not only the team's decision: a sponsored capstone project may come with terms about who owns the work.

**Do not add a licence file until this is settled with the sponsor and the advisor.**

## Options

### A. No licence, repository stays private

Nothing to decide. Each team member can still describe the project and their own contribution in a portfolio. Sharing the files themselves needs permission from whoever owns the work.

### B. A permissive open-source licence for the firmware, an open hardware licence for the boards

Software licences such as MIT or Apache-2.0 were written for code. Hardware design files are usually released under a licence written for hardware, such as one of the CERN Open Hardware Licences. Documentation is often released under a Creative Commons licence. So a public repository may carry more than one.

### C. Public to read, all rights reserved

The repository is visible, with a notice that it may not be reused without permission. Works as a portfolio piece without giving anything away.

## Things that constrain the choice

- **Any agreement between LSU, the Capstone Design Program and the sponsor** about ownership of project work.
- **Third-party code and its licences:** STM32 HAL and CubeMX output, any USB stack, the Unity test framework. Each has terms that must be kept if the code is redistributed. Keep their licence files next to them.
- **Last year's work in `legacy/`.** It was made by a different team. Publishing it needs their agreement or the program's.
- **Third-party component models and footprints**, which sometimes come with their own terms.

## What we need to find out first

1. **Ask the advisor** what the Capstone Design Program says about intellectual property and publishing project work.
2. **Ask the sponsor** whether the design should be public, private or handed over.
3. **List the third-party code** in the repository and its licences, once there is some.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
