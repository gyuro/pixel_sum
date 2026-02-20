# PixelSum

A tiny C++17 library that builds summed-area tables (integral images) so rectangular queries over pixel buffers are O(1). It supports 8- and 16-bit pixels via a templated `PixelSum<T, S>` that stores both the raw pixels and two integral images: one for sums and one for non-zero counts.

## Features
- Constant-time sum, average, non-zero count, and non-zero average for any axis-aligned window
- Bounds clamping and coordinate swapping built in
- `PixelSumU8` and `PixelSumU16` aliases for common 8- and 16-bit use
- Self-contained: only the standard library and CMake are required

## Build and run
```bash
cmake --preset default
cmake --build --preset default
ctest --preset default
```

Manual fallback:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --parallel
./build/PixelSumTest
```

## Usage
```cpp
#include <pixel_sum/PixelSum.h>

std::vector<std::uint8_t> pixels(width * height, 128);
PixelSumU8 ps(pixels.data(), width, height);

auto total = ps.getPixelSum(0, 0, width - 1, height - 1);
auto avg = ps.getPixelAverage(10, 10, 20, 20);
auto nzCount = ps.getNonZeroCount(0, 0, width - 1, height - 1);
auto nzAvg = ps.getNonZeroAverage(5, 5, 15, 15);
```

## Project layout
- `include/pixel_sum/` — public library headers
- `src/` — implementation
- `tests/` — test entrypoint and support headers
- `.github/workflows/` — CI/quality automation

## How it works
Construction computes two integral images (sum and non-zero mask). Each query reads four corners and combines them (`D - B - C + A`), so query time stays constant while memory overhead is linear in the number of pixels.

## Code quality

```bash
cmake --build --preset default --target format
cmake --build --preset default --target format-check
cmake --build --preset default --target tidy
cmake --build --preset default --target cppcheck
```

## CI pipeline

GitHub Actions (`.github/workflows/ci.yml`) runs:

1. `lint` — clang-format check, cppcheck, clang-tidy (advisory)
2. `build_test` — build + ctest
3. `coverage` — gcovr report artifacts
4. `packaging` — CPack `.tar.gz` package artifact

## Notes
- Dimensions are limited to 4096 x 4096 to keep intermediate sums within `uint32_t` for 8-bit pixels; switch to `PixelSumU16` for larger ranges or higher bit depth.
- The minimal test harness in `tests/PixelSumTest.cpp` exercises edge cases and can be extended with additional `TEST` blocks.
