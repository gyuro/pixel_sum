#pragma once

#include <vector>

template <typename T, typename S>
class PixelSum {
    enum { MAX_WIDTH = 4096,
        MAX_HEIGHT = 4096 };

public:
    explicit PixelSum(const T* buffer, int width, int height);
    ~PixelSum(void);
    PixelSum(const PixelSum&);
    PixelSum& operator=(const PixelSum&);

    S getPixelSum(int x0, int y0, int x1, int y1) const;
    double getPixelAverage(int x0, int y0, int x1, int y1) const;
    S getNonZeroCount(int x0, int y0, int x1, int y1) const;
    double getNonZeroAverage(int x0, int y0, int x1, int y1) const;

    operator bool() const;

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
    S getSummedArea(const std::vector<S>& data, int x0, int y0, int x1, int y1) const;
};

typedef PixelSum<std::uint8_t, std::uint32_t> PixelSumU8;
typedef PixelSum<std::uint16_t, std::uint64_t> PixelSumU16;

template <typename T>
inline void swap_if_a_greater_than_b(T& a, T& b)
{
    if (a > b) {
        T tmp = a;
        a = b;
        b = tmp;
    }
}
