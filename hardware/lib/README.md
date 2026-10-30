# hardware/lib

Symbols, footprints and 3D models that both KiCad projects share and that are not in KiCad's standard libraries.

Suggested layout:

```text
lib/
├── tigerbass2.kicad_sym        project symbol library
├── tigerbass2.pretty/          project footprint library, one .kicad_mod per footprint
└── 3d/                         STEP models referenced by the footprints
```

## Rules

- **Reference these libraries with a path relative to the project** (`${KIPRJMOD}/../lib/...`), never an absolute path on your own computer. An absolute path works for you and breaks the project for everyone else.
- **Check every new footprint against the datasheet's recommended land pattern** before it goes on a board, and say in the pull request which page you checked it against. A wrong footprint is the most common way to waste a board order.
- **Fill in the fields on every symbol:** value, footprint, datasheet link, manufacturer, manufacturer part number.
- Use a part from KiCad's standard libraries when it exists and is correct. Add here only what is missing.

## Who edits here

Anyone adding a part. Symbol and footprint libraries are easier to merge than board files, but still tell the team when you are adding to them.
