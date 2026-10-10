# firmware

The code that runs on the controller board's microcontroller.

| Folder | Contents | Builds on |
| --- | --- | --- |
| [`app/`](app/) | The logic: which note, what velocity, when to send what. Portable C with no vendor headers | A PC and the board |
| [`bsp/`](bsp/) | The board support package: STM32 HAL glue and CubeMX-generated code | The board only |
| [`tests/`](tests/) | Unit tests for `app/`, with fake hardware | A PC only |

## Why it is split this way

The firmware has one hard job: turn a pluck into a MIDI message within a few milliseconds, every time. The logic that does that is the part most likely to have bugs, and bugs in it are slow to find on a real instrument.

So the logic lives in `app/` and reaches hardware only through a handful of small interfaces: drive a string, read the fret lines, read a pickup sample, get the time, queue a MIDI message. `bsp/` implements those interfaces on the real board. `tests/` implements them with fakes that replay a scripted sequence of fret readings and pickup samples.

That gives two things. The logic can be tested on a laptop in seconds, including cases that are hard to play on purpose, such as two strings plucked in the same scan. And a change of microcontroller or ADC only touches `bsp/`.

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
