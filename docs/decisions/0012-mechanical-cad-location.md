# 0012: Where mechanical CAD lives

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)`. The mechanical engineers decide |
| Needed by | Before the first mechanical design file is shared |
| Requirements | None. This is a working agreement |
| Analysis | None needed |

## The question

Do the native mechanical CAD files live in this repository, or somewhere else with only exports here?

## Why it matters

The mechanical and electrical designs depend on each other: board outlines, mounting holes, connector positions, component heights. Each side needs to find the other's current design without asking. Git is good at text and poor at large binary files: it cannot show what changed in a CAD file, cannot merge two people's edits, and keeps every version forever, so the repository grows with every save.

## Options

### A. Exports only in this repo, native files in the CAD tool's own storage

The mechanical engineers work wherever their CAD tool works best: its cloud workspace, a university drive, or a data-management system. They commit a STEP file and a PDF drawing here at each milestone. The repository stays small. The electrical side always has something it can open. Costs: the exports can lag behind the real design, and the native files' history is somewhere else.

### B. Native files in this repo, with Git LFS

Everything in one place with one history. Git LFS keeps the large files out of the main repository data. Needs every team member to install Git LFS, and GitHub's LFS storage and bandwidth allowances have limits that should be checked first. Two people still cannot edit the same part at once.

### C. Native files in this repo, without LFS

Simplest to set up. The repository grows quickly and cloning gets slow. Not recommended for anything beyond a few small files.

## How to choose

1. Which CAD tool the mechanical engineers use, and how it prefers to store files.
2. Whether the Capstone Design Program requires design files to be kept somewhere in particular.
3. How often the electrical side needs an up-to-date mechanical model.
4. Whether the sponsor expects to receive native files at the end, and in what form.

## What we need to find out first

1. **The CAD tool and its file sizes.**
2. **GitHub's current LFS allowance** for this account, if option B is on the table.
3. **Program requirements** for archiving design files.

## Whatever is chosen

- A STEP export and a dimensioned PDF of anything the electronics mount to or pass through go in [`mechanical/`](../../mechanical/), with the revision in the file name.
- The dimensions shared with the electronics are listed in `mechanical/README.md` and kept current.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
