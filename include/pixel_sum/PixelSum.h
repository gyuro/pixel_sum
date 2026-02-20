#pragma once

#include <cstdint>
#include <utility>
#include <vector>

template <typename T, typename S>
class PixelSum {
    static constexpr int MAX_WIDTH = 4096;
    static constexpr int MAX_HEIGHT = 4096;

public:
    explicit PixelSum(const T* buffer, int width, int height);
    ~PixelSum() = default;
    PixelSum(const PixelSum&) = default;
    PixelSum(PixelSum&&) noexcept = default;
    PixelSum& operator=(const PixelSum&) = default;
    PixelSum& operator=(PixelSum&&) noexcept = default;

    [[nodiscard]] S getPixelSum(int x0, int y0, int x1, int y1) const;
    [[nodiscard]] double getPixelAverage(int x0, int y0, int x1, int y1) const;
    [[nodiscard]] S getNonZeroCount(int x0, int y0, int x1, int y1) const;
    [[nodiscard]] double getNonZeroAverage(int x0, int y0, int x1, int y1) const;

    explicit operator bool() const noexcept;

private:
    int width_ { 0 };
    int height_ { 0 };

    // pixel data
    std::vector<T> pixel_data_ {};

    // sparse matrix for existent flags
    std::vector<S> nonzero_data_ {};

    // each pixel represents the cumulative sum of corresponding input pixel with
    // all pixels above and to the left of input pixel.
    std::vector<S> summed_data_ {};

    void swap(int& x0, int& y0, int& x1, int& y1) const;
    bool clampBound(int& x0, int& y0, int& x1, int& y1) const;
    [[nodiscard]] S getSummedArea(const std::vector<S>& data, int x0, int y0, int x1, int y1) const;
};

using PixelSumU8 = PixelSum<std::uint8_t, std::uint32_t>;
using PixelSumU16 = PixelSum<std::uint16_t, std::uint64_t>;

template <typename T>
inline void swap_if_a_greater_than_b(T& a, T& b)
{
    if (a > b) {
        std::swap(a, b);
    }
}
