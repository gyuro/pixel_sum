# PixelSum

A tiny C++17 library that builds summed-area tables (integral images) so rectangular queries over pixel buffers are O(1). It supports 8- and 16-bit pixels via a templated `PixelSum<T, S>` that stores both the raw pixels and two integral images: one for sums and one for non-zero counts.

## Features
- Constant-time sum, average, non-zero count, and non-zero average for any axis-aligned window
- Bounds clamping and coordinate swapping built in
- `PixelSumU8` and `PixelSumU16` aliases for common 8- and 16-bit use
- Self-contained: only the standard library and CMake are required

## Build and run
```bash
cmake -S . -B build
cmake --build build
./build/PixelSumTest
```

## Usage
```cpp
std::vector<std::uint8_t> pixels(width * height, 128);
PixelSumU8 ps(pixels.data(), width, height);

auto total = ps.getPixelSum(0, 0, width - 1, height - 1);
auto avg = ps.getPixelAverage(10, 10, 20, 20);
auto nzCount = ps.getNonZeroCount(0, 0, width - 1, height - 1);
auto nzAvg = ps.getNonZeroAverage(5, 5, 15, 15);
```

## How it works
Construction computes two integral images (sum and non-zero mask). Each query reads four corners and combines them (`D - B - C + A`), so query time stays constant while memory overhead is linear in the number of pixels.

## Notes
- Dimensions are limited to 4096 x 4096 to keep intermediate sums within `uint32_t` for 8-bit pixels; switch to `PixelSumU16` for larger ranges or higher bit depth.
- The minimal test harness in `PixelSumTest.cpp` exercises edge cases and can be extended with additional `TEST` blocks.
