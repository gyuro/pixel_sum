#include "PixelSum.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

// In case of huge data is given, then it should take a fairly long time.
template <typename T, typename S, typename Transformer>
void IntegralImage(const std::vector<T>& source,
    int width,
    int height,
    std::vector<S>& target,
    Transformer transformer)
{
    auto source_iter = source.cbegin();
    auto target_iter = target.begin();

    // The summed matrix is calculated from top left corner
    // I(x, y) = i(x, y) + I(x, y - 1) + I(x - 1, y) - I(x - 1, y - 1)

    // fill first row
    *target_iter++ = transformer(*source_iter++);
    for (int x = 1; x < width; x++) {
        *target_iter++ = *(target_iter - 1) + transformer(*source_iter++);
    }

    // fill rest of row
    for (int y = 1; y < height; y++) {
        *target_iter++ = transformer(*source_iter++) + *(target_iter - width);
        for (int x = 1; x < width; x++) {
            *target_iter++ = transformer(*source_iter++) + *(target_iter - width) + *(target_iter - 1) - *(target_iter - width - 1);
        }
    }
}

template <typename T, typename S>
void IntegralImage(const std::vector<T>& source, int width, int height, std::vector<S>& target)
{
    IntegralImage(source,
        width,
        height,
        target,
        [](const T& val) -> S { return static_cast<S>(val); });
}

template <typename T, typename S>
PixelSum<T, S>::PixelSum(const T* buffer, int width, int height)
    : width_(width)
    , height_(height)
{
    if (width_ <= 0 || width_ > MAX_WIDTH || height_ <= 0 || height_ > MAX_HEIGHT) {
        throw std::runtime_error("Dimension is out of bound");
    }

    const auto dimension = static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_);

    // copy buffer to pixel data
    pixel_data_ = std::vector<T>(buffer, buffer + dimension);

    // non-zero count for sparce matrix
    nonzero_data_ = std::vector<S>(dimension);
    IntegralImage<T, S>(
        pixel_data_, width_, height_, nonzero_data_, [](const T& val) -> T {
            return val > 0;
        });

    // cumulative matrix
    summed_data_ = std::vector<S>(dimension);
    IntegralImage(pixel_data_, width_, height_, summed_data_);
}

template <typename T, typename S>
S PixelSum<T, S>::getPixelSum(int x0, int y0, int x1, int y1) const
{
    swap(x0, y0, x1, y1);
    if (!clampBound(x0, y0, x1, y1)) {
        return 0;
    }

    return getSummedArea(summed_data_, x0, y0, x1, y1);
}

template <typename T, typename S>
double PixelSum<T, S>::getPixelAverage(int x0, int y0, int x1, int y1) const
{
    swap(x0, y0, x1, y1);
    int count = (x1 - x0 + 1) * (y1 - y0 + 1);

    if (!clampBound(x0, y0, x1, y1)) {
        return 0.0;
    }

    double sum = static_cast<double>(getPixelSum(x0, y0, x1, y1));

    return sum / count;
}

template <typename T, typename S>
S PixelSum<T, S>::getNonZeroCount(int x0, int y0, int x1, int y1) const
{
    swap(x0, y0, x1, y1);
    if (!clampBound(x0, y0, x1, y1)) {
        return 0;
    }

    return getSummedArea(nonzero_data_, x0, y0, x1, y1);
}

template <typename T, typename S>
double PixelSum<T, S>::getNonZeroAverage(int x0, int y0, int x1, int y1) const
{
    double sum = static_cast<double>(getPixelSum(x0, y0, x1, y1));
    auto count = getNonZeroCount(x0, y0, x1, y1);

    return count > 0 ? (sum / count) : 0;
}

template <typename T, typename S>
PixelSum<T, S>::operator bool() const noexcept
{
    return !pixel_data_.empty() && !nonzero_data_.empty() && !summed_data_.empty();
}

template <typename T, typename S>
void PixelSum<T, S>::swap(int& x0, int& y0, int& x1, int& y1) const
{
    swap_if_a_greater_than_b(x0, x1);
    swap_if_a_greater_than_b(y0, y1);
}

template <typename T, typename S>
bool PixelSum<T, S>::clampBound(int& x0, int& y0, int& x1, int& y1) const
{
    if ((x0 < 0 && x1 < 0) || (x0 >= width_ && x1 >= width_) || (y0 < 0 && y1 < 0) || (y0 >= height_ && y1 >= height_)) {
        return false;
    }

    x0 = std::max(x0, 0);
    y0 = std::max(y0, 0);
    x1 = std::min(x1, width_ - 1);
    y1 = std::min(y1, height_ - 1);

    return true;
}

template <typename T, typename S>
S PixelSum<T, S>::getSummedArea(const std::vector<S>& data,
    int x0,
    int y0,
    int x1,
    int y1) const
{
    /*
   * (x0, y0)        (x1, y0)
   *    A---------------B
   *    |               |
   *    | D - B - C + A |
   *    |               |
   *    C---------------D
   * (x0, y1)        (x1, y1)
   */

    S A = 0, B = 0, C = 0, D = 0;
    D = data[y1 * width_ + x1];
    if (x0 >= 1 && y0 >= 1) {
        A = data[(y0 - 1) * width_ + (x0 - 1)];
        C = data[y1 * width_ + (x0 - 1)];
        B = data[(y0 - 1) * width_ + x1];
    } else if (x0 >= 1) {
        C = data[y1 * width_ + (x0 - 1)];
    } else if (y0 >= 1) {
        B = data[(y0 - 1) * width_ + x1];
    }

    return D - B - C + A;
}

template class PixelSum<std::uint8_t, std::uint32_t>;
template class PixelSum<std::uint16_t, std::uint64_t>;
