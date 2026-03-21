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

# 1. Navigate to the TIC project directory
cd M:\git\GTest_Trial\TIC

# 2. Create build directory (if not exists)
if (!(Test-Path build)) { mkdir build }
cd build

# 3. Configure the project with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 4. Build the project entirely (including tests)
cmake --build . --config Release

# 5. Run the test suite
.\test_tic.exe

# 6. Generate HTML coverage report
python -m gcovr -r .. --object-directory CMakeFiles/test_tic.dir --html --html-details -o coverage.html

# 7. Open the coverage report in your browser
start coverage.html
