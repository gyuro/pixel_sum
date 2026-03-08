#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

template <typename T, typename S>
class PixelSum {
    static constexpr int kMaxWidth = 4096;
    static constexpr int kMaxHeight = 4096;

public:
    explicit PixelSum(const T* buffer, int width, int height);
    explicit PixelSum(std::span<const T> buffer, int width, int height);

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
    int width_{0};
    int height_{0};

    std::vector<T> pixel_data_{};
    std::vector<S> nonzero_data_{};
    std::vector<S> summed_data_{};

    static void normalizeBounds(int& x0, int& y0, int& x1, int& y1);
    [[nodiscard]] bool clampBounds(int& x0, int& y0, int& x1, int& y1) const;
    [[nodiscard]] static std::size_t indexOf(int x, int y, int width) noexcept;
    [[nodiscard]] S getSummedArea(std::span<const S> data, int x0, int y0, int x1, int y1) const;
};

using PixelSumU8 = PixelSum<std::uint8_t, std::uint32_t>;
using PixelSumU16 = PixelSum<std::uint16_t, std::uint64_t>;
