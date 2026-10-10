# docs

The written record of the design: what it must do, how it is built, why it is built that way, and how we know it works.

| Path | Contents |
| --- | --- |
| [`requirements.md`](requirements.md) | What the instrument must do, each item with an ID and a way to verify it |
| [`architecture.md`](architecture.md) | Block diagrams and the interfaces between boards |
| [`decisions/`](decisions/) | One file per design decision: the question, the options, what was chosen and why |
| [`analysis/`](analysis/) | Trade studies and calculations that feed the decisions |
| [`test/`](test/) | Test plans, procedures and results |
| [`legacy/`](legacy/) | Notes on last year's Tiger Bass 1 |
| [`copilot/`](copilot/) | The prompts we give the Copilot agent |

## How these fit together

A **requirement** says what must be true. A **decision** records how we chose to meet it, and points at the **analysis** that justified the choice. A **test** shows the built instrument meets the requirement. Each links to the others by ID, so a reviewer or a judge can follow any claim back to its evidence.

## Writing style

- Markdown, with diagrams as Mermaid code blocks so they render on GitHub and show up in a diff.
- Numbers carry units and say where they came from: a measurement (with date and equipment), a datasheet (with page), or a calculation (with the script).
- `TBD(team)` marks anything not decided. Search the repo for it to see what is still open.
