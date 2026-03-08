#include "pixel_sum/pixel_sum.hpp"

#include <algorithm>
#include <stdexcept>

namespace {
template <typename T, typename S, typename Transformer>
void buildIntegralImage(std::span<const T> source,
                        int width,
                        int height,
                        std::span<S> target,
                        Transformer transformer)
{
    const auto at = [width](int x, int y) -> std::size_t {
        return static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x);
    };

    target[at(0, 0)] = transformer(source[at(0, 0)]);

    for (int x = 1; x < width; ++x) {
        target[at(x, 0)] = target[at(x - 1, 0)] + transformer(source[at(x, 0)]);
    }

    for (int y = 1; y < height; ++y) {
        target[at(0, y)] = target[at(0, y - 1)] + transformer(source[at(0, y)]);
        for (int x = 1; x < width; ++x) {
            target[at(x, y)] = transformer(source[at(x, y)]) + target[at(x, y - 1)] + target[at(x - 1, y)] -
                               target[at(x - 1, y - 1)];
        }
    }
}

template <typename T, typename S>
void buildIntegralImage(std::span<const T> source, int width, int height, std::span<S> target)
{
    buildIntegralImage<T, S>(source, width, height, target, [](const T value) -> S { return static_cast<S>(value); });
}
} // namespace

template <typename T, typename S>
PixelSum<T, S>::PixelSum(const T* buffer, int width, int height)
    : PixelSum(std::span<const T>(buffer, static_cast<std::size_t>(width) * static_cast<std::size_t>(height)), width, height)
{
}

template <typename T, typename S>
PixelSum<T, S>::PixelSum(std::span<const T> buffer, int width, int height)
    : width_(width)
    , height_(height)
{
    if (width_ <= 0 || width_ > kMaxWidth || height_ <= 0 || height_ > kMaxHeight) {
        throw std::runtime_error("Dimension is out of bound");
    }

    const auto dimension = static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_);
    if (buffer.size() < dimension) {
        throw std::runtime_error("Buffer size is smaller than width*height");
    }

    pixel_data_.assign(buffer.begin(), buffer.begin() + static_cast<std::ptrdiff_t>(dimension));

    nonzero_data_.assign(dimension, S{});
    buildIntegralImage<T, S>(std::span<const T>(pixel_data_),
                             width_,
                             height_,
                             std::span<S>(nonzero_data_),
                             [](const T value) -> S { return value > T{} ? S{1} : S{0}; });

    summed_data_.assign(dimension, S{});
    buildIntegralImage<T, S>(std::span<const T>(pixel_data_), width_, height_, std::span<S>(summed_data_));
}

template <typename T, typename S>
S PixelSum<T, S>::getPixelSum(int x0, int y0, int x1, int y1) const
{
    normalizeBounds(x0, y0, x1, y1);
    if (!clampBounds(x0, y0, x1, y1)) {
        return S{};
    }

    return getSummedArea(std::span<const S>(summed_data_), x0, y0, x1, y1);
}

template <typename T, typename S>
double PixelSum<T, S>::getPixelAverage(int x0, int y0, int x1, int y1) const
{
    normalizeBounds(x0, y0, x1, y1);
    if (!clampBounds(x0, y0, x1, y1)) {
        return 0.0;
    }

    const auto count = static_cast<double>((x1 - x0 + 1) * (y1 - y0 + 1));
    const auto sum = static_cast<double>(getSummedArea(std::span<const S>(summed_data_), x0, y0, x1, y1));
    return count > 0.0 ? (sum / count) : 0.0;
}

template <typename T, typename S>
S PixelSum<T, S>::getNonZeroCount(int x0, int y0, int x1, int y1) const
{
    normalizeBounds(x0, y0, x1, y1);
    if (!clampBounds(x0, y0, x1, y1)) {
        return S{};
    }

    return getSummedArea(std::span<const S>(nonzero_data_), x0, y0, x1, y1);
}

template <typename T, typename S>
double PixelSum<T, S>::getNonZeroAverage(int x0, int y0, int x1, int y1) const
{
    const auto sum = static_cast<double>(getPixelSum(x0, y0, x1, y1));
    const auto count = getNonZeroCount(x0, y0, x1, y1);
    return count > S{} ? (sum / static_cast<double>(count)) : 0.0;
}

template <typename T, typename S>
PixelSum<T, S>::operator bool() const noexcept
{
    return !pixel_data_.empty() && !nonzero_data_.empty() && !summed_data_.empty();
}

template <typename T, typename S>
void PixelSum<T, S>::normalizeBounds(int& x0, int& y0, int& x1, int& y1)
{
    if (x0 > x1) {
        std::swap(x0, x1);
    }
    if (y0 > y1) {
        std::swap(y0, y1);
    }
}

template <typename T, typename S>
bool PixelSum<T, S>::clampBounds(int& x0, int& y0, int& x1, int& y1) const
{
    if ((x0 < 0 && x1 < 0) || (x0 >= width_ && x1 >= width_) || (y0 < 0 && y1 < 0) || (y0 >= height_ && y1 >= height_)) {
        return false;
    }

    x0 = std::clamp(x0, 0, width_ - 1);
    y0 = std::clamp(y0, 0, height_ - 1);
    x1 = std::clamp(x1, 0, width_ - 1);
    y1 = std::clamp(y1, 0, height_ - 1);
    return true;
}

template <typename T, typename S>
std::size_t PixelSum<T, S>::indexOf(int x, int y, int width) noexcept
{
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x);
}

template <typename T, typename S>
S PixelSum<T, S>::getSummedArea(std::span<const S> data, int x0, int y0, int x1, int y1) const
{
    const auto idx = [this](int x, int y) -> std::size_t { return indexOf(x, y, width_); };

    const S d = data[idx(x1, y1)];
    const S b = y0 > 0 ? data[idx(x1, y0 - 1)] : S{};
    const S c = x0 > 0 ? data[idx(x0 - 1, y1)] : S{};
    const S a = (x0 > 0 && y0 > 0) ? data[idx(x0 - 1, y0 - 1)] : S{};

    return d - b - c + a;
}

template class PixelSum<std::uint8_t, std::uint32_t>;
template class PixelSum<std::uint16_t, std::uint64_t>;
