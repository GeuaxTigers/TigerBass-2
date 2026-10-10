# hardware/sim

Circuit simulations that back a design decision: LTspice schematics and anything similar.

## Layout

One folder per question, named for the question:

```text
sim/
├── string-drive-levels/
│   ├── string-drive-levels.asc
│   └── README.md
└── pickup-front-end/
    ├── pickup-front-end.asc
    └── README.md
```

## Each simulation folder has a README that says

1. The question the simulation answers.
2. What was assumed: component models, source amplitude and frequency, load.
3. The result, as a number with units.
4. A screenshot of the plot the result was read from.

Without that, a `.asc` file is a drawing nobody else can check.

## What gets committed

Commit the `.asc` schematic, any `.asy` symbols and third-party model files it needs, and the README with its screenshot. Do not commit `.raw`, `.log` or `.net` files: LTspice regenerates them on every run and `.gitignore` excludes them.

## Starting points from last year

Last year's team simulated the string-on-fret logic levels and the amplitude detection circuit in LTspice. The numbers they reported are in [`docs/legacy/tigerbass1-notes.md`](../../docs/legacy/tigerbass1-notes.md). The simulation files themselves are not in this repo yet.
