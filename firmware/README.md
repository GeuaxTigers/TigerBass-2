# firmware

The code that runs on the controller board's microcontroller.

## Running the tests

```bash
cmake -S firmware -B build/host -DTB2_BUILD_TESTS=ON
cmake --build build/host
ctest --test-dir build/host --output-on-failure
```

These commands start working once `firmware/CMakeLists.txt` exists. Creating it is the "portable firmware core" task in [`docs/copilot/kickoff-prompt.md`](../docs/copilot/kickoff-prompt.md).

## Rules

The full list is under "Firmware rules" in [`.github/copilot-instructions.md`](../.github/copilot-instructions.md). They apply to people as well as to Copilot. The short version:

- Nothing in the scan path waits.
- No dynamic memory.
- Every `app/` module has tests.
- Derive tables from the tuning and fret count instead of typing them.
- Put the unit in the name: `threshold_mv`, `window_us`.
