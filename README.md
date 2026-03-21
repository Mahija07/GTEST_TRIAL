# GTest_Trial - TIC Testing Project

## Overview
Comprehensive Google Test suite for the TIC (Torque Intervention Control) system with 96%+ code coverage.

## Test Coverage
- **tic.cpp**: 96.55% line coverage
- **test_tic.cpp**: 100% line coverage
- **Overall**: 82.71% across all files

## Build & Run
```bash
cd TIC/build
cmake ..
cmake --build . --config Release
./test_tic.exe
```

## Coverage Report
Open `build/coverage.html` for detailed line-by-line coverage visualization.

## Files
- `test_tic.cpp` - 5 comprehensive tests covering state machine, rate limiting, fault handling, PWM generation
- `CMakeLists.txt` - CMake build configuration with coverage instrumentation
- `Makefile` - Legacy makefile (see CMakeLists.txt for preferred build)
