# SkyRoof Unix/C Port (MVP)

This folder contains the first implementation step for a Unix-native C port.

## implemented MVP scope

- core C library (`skyroof_core`) with:
  - logging primitives
  - sample pipeline ring buffer
  - CAT frequency command parsing (`F`/`f` subset)
  - Doppler shift calculation helper for satellite tracking logic
- Unix adapter layer (`skyroof_unix_adapter`) with a headless audio stub
- CLI tool (`skyroof_cli`) for headless validation workflows
- compatibility-oriented tests wired into `ctest`

## deferred for later phases

- full SDR streaming integration with SoapySDR
- full FT4 encode/decode orchestration
- full CAT command matrix and rig backends
- GUI parity with the Windows WinForms interface
- speech announcements and advanced desktop integration

## dependencies

Required:

- CMake 3.20+
- C11 compiler (gcc or clang)

Optional auto-detected dependencies (for later feature phases):

- FFTW3f
- SoapySDR
- liquid-dsp

## build and test

```sh
./scripts/build-unix.sh
```

or manually:

```sh
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## migration sequence represented by this implementation

1. headless core and adapter scaffolding
2. CLI behavior checks and deterministic tests
3. progressive replacement of stubs with Unix integrations
4. eventual UI layer selection and parity work
