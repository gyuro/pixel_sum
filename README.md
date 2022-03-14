# PixelSum


## 1. General

This is a real-life example of a data structure (“pixel sum”) that we use all the time in various computer vision and graphics work. The code is highly performance critical: it is called millions of times per image, often with very large kernels (thousands of pixels).

Hence, PixelSum has to be implemented in efficient ways so that we can tackle into tremendous tries.

## 2. Descriptions

### PixelSum class

As I took change of data type into account to begin with, so the data structure could be represented as a template class to cope with a different type later on conviniently.

PixelSum class receives as its input an 8-bits two-dimensional pixel buffer (grayscale data or one channel at a time,  possibly huge data) and size of image, the constructor should take its own copy of the input. After that, cumulative data that are non-zero data and summed-area data to make later summation or counting operation efficienty are supposed to be then calculated with integral image computation.

Since the actual pixel buffer could have either multiple channels or bigger data type, in order to tackle into taking own flexiblity then PixelSum class was implemented as a template class. Please see below an example. 

```template<typename T, typename S> class PixelSum { ... }``` where T is defined as a data type that we use such as uint8_t or uint16_t, and S is defined as a summed data type that we use such as uint32_t or uint64_t.
- PixelSumU8 : PixelSum<uint8_t, uint32_t> can be used when a given pixel consists of 8-bits
- PixelSumU16 : PixelSum<uint16_t, uint64_t> can be used when a given pixel consists of 16-bits

PixelSum provides 3 different constructors that are a parametrized constructor, a copy constructor and an assignment operator to instantiate its own class.

```
explicit PixelSum(const T* buffer, int width, int height); // parametrized constructor
PixelSum(const PixelSum&); // copy constructor
PixelSum& operator=(const PixelSum&); // assignment operator
```

PixelSum provides 4 essential functions to deal with pixels inside, but for computing the regional numbers I get the integral computation processed in costructor, the following functions then will take in a fairly short.

```
S getPixelSum(int x0, int y0, int x1, int y1) const;
double getPixelAverage(int x0, int y0, int x1, int y1) const;
S getNonZeroCount(int x0, int y0, int x1, int y1) const;
double getNonZeroAverage(int x0, int y0, int x1, int y1) const;
```

As the current version gives me some constraints that takes the input size less than or equal to 4096 x 4096 and a pixel is represented as 8-bits, therefore a possible maximum value in summation can be a 2^32 (2^12 x 2^12 x 2^8) value. On the other hands when newer one accepts the input size that is greater than 4096 x 4096 and a pixel has 16-bits, in such case a possible maximum value in summation can be a 2^64 (2^24 x 2^24 x 2^16) value.

According to above conditions, *uint8_t* can be used in a case that uses 8-bits per pixel and 4096^2 size as maximum, whereas *uint16_t* can be used in a case that uses 16-bits per pixel and bigger than 4096^2 size.


### Utilities macros

#### Test Utilities

Below TEST macro is useful when a test case needs to be added newly.

```
#define TEST(test_case_name, test_name)
```

In case a test is written by using above macro, then you can run it with one of below CALL macros. CALL_TEST_TIMED macro will report as an output to take elapsed time.
```
#define CALL_TEST(test_case_name, test_name)
#define CALL_TEST_TIMED(test_case_name, test_name)
```

In the written test cases, the below EXPECT comparisons macro can be called to compare and report a result for actual and expected values.
```
#define EXPECT_NEAR(actual, expected, precision)  
#define EXPECT_EQ(actual, expected)
#define EXPECT_GE(val1, val2)
#define EXPECT_GT(val1, val2)
#define EXPECT_NO_THROW(method) 
#define EXPECT_THROW(method, exception)
```

#### Time Utilities

In order to measure how long it takes, below TIME macros can be used at the point that needs to be checked.

```
#define TIME_START()
#define TIME_END()
#define TIME_ELAPSED()
```

As mentioned in Test Utilities, in CALL_TEST_TIMED macro calls above TIME macros in a timely manner.


## 3. Summed Area - Integral Image

A summed-area table is a data structure and algorithm for quickly and efficiently generating the sum of values in a rectangular subset of a grid. In the image processing domain, it is also known as an integral image.

Please see: [Summed-area table](https://en.wikipedia.org/wiki/Summed-area_table)

## 4. Usage

### Prerequisites

The development environments should be kept by:

- A modern `C/C++` compiler, preferably `c++17`
- CMake 3.1+ installed
- No thirdparty required

### Project Structure

The file structure is organized as below.

- PixelSum.cpp/h : All definitions on PixelSum class
- PixelSumTest.cpp : PixelSum unit tests
- TestUtility.h : Test utility to perform unit tests suitably
- TimeUtility.h : Time utility to take the elapsed time
- Common.h : Color keys
- README.md : README
- CMakeLists.txt : CMake configuration file

### Building Manually

`$> rm -rf build && mkdir build`

`$> cd build`

`$> cmake ..`

`$> make && make install`

### Running Unit Tests

On Linux: `$> ./PixelSumTest`

On Window: `$> PixelSumTest.exe`

Running PixelSumTest leads to following log messages.

```
PixelSumTest_GivenWindow_WhenSwap_ThenExpectedSwappedWindowIsReturned
 Success: val1 100 >= val2 50
 It took 2.3924 ms

PixelSumTest_GivenPixelsInsideWindow_WhenGetPixelAverage_ThenExpectedAverageIsReturned
 Success: actual 1.16667 which expects 1.166 (0.001 eps)
 It took 8.6072 ms

PixelSumTest_GivenPixelsInsideWindow_WhenGetPixelAveragebySwappedPositions_ThenExpectedAverageIsReturned
 Success: actual 1.16667 which expects 1.166 (0.001 eps)
 It took 7.4885 ms

PixelSumTest_GivenPixelsOverlappedWindow_WhenGetPixelAverage_ThenExpectedAverageIsReturned
 Success: actual 0.111111 which expects 0.111111 (1e-06 eps)
 It took 8.0077 ms

PixelSumTest_GivenPixelsOverlappedWindow2_WhenGetPixelAverage_ThenExpectedAverageIsReturned
 Success: actual 0.25 which expects 0.25 (0 eps)
 It took 4.1249 ms

PixelSumTest_GivenPixelsInsideWindow_WhenGetNonZeroAverage_ThenExpectedAverageIsReturned
 Success: actual 1.16667 which expects 1.166 (0.001 eps)
 Success: actual 2.33333 which expects 2.333 (0.001 eps)
 It took 11.8184 ms

PixelSumTest_GivenPixelsInBound_WhenContruction_ThenNoErrorIsThrown
 Success: No exception is thrown
 It took 47.9639 ms

PixelSumTest_GivenPixelsOutOfDimension_WhenContruction_ThenRuntimeErrorIsThrown
 Success: Dimension is out of bound
 It took 33.6827 ms

PixelSumTest_GivenPixels_WhenGetNonZeroCount_ThenExpectedNonZeroCountIsReturned
 Success: actual 65536 which expects 65536 (0 eps)
 It took 208.088 ms

PixelSumTest_GivenPixels_WhenGetNonZeroAverage_ThenExpectedNonZeroAverageIsReturned
 Success: actual 128 which expects 128 (0 eps)
 It took 201.913 ms
```

## 5. Examples

The following is an example that has an input data are taken by maximum size and filled as zero at all. After that, 4 fundamental functions are called to be compared if they are performed well.

```
TEST(PixelSumTest, GivenMaxSize4096x4096WithPixelValueZero_WhenCallAllGetters_ThenExpectedValuesAreReturned)
{
    int pixel_val = 0;
    std::uint32_t width = 4096;
    std::uint32_t height = 4096;
    std::vector<std::uint8_t> data(4096 * 4096, pixel_val);

    auto pixel_sum = PixelSumU8(data.data(), width, height);

    auto actual_pixel_average = pixel_sum.getPixelAverage(0, 0, width - 1, height - 1);
    auto actual_pixel_sum = pixel_sum.getPixelSum(0, 0, width - 1, height - 1);
    auto actual_nonzero_average = pixel_sum.getNonZeroAverage(0, 0, width - 1, height - 1);
    auto actual_nonzero_count = pixel_sum.getNonZeroCount(0, 0, width - 1, height - 1);

    EXPECT_EQ(actual_pixel_average, pixel_val);
    EXPECT_EQ(actual_pixel_sum, width * height * pixel_val);
    EXPECT_EQ(actual_nonzero_average, pixel_val);
    EXPECT_EQ(actual_nonzero_count, 0);
}
```

The following is an example that should throw an exception due to out of dimension.
```
TEST(PixelSumTest, GivenPixelsOutOfDimension_WhenContruction_ThenRuntimeErrorIsThrown)
{
    std::uint32_t width = 4096 * 2;
    std::uint32_t height = 4096 * 2;
    std::vector<std::uint8_t> data(width * height);

    EXPECT_THROW(PixelSumU8(data.data(), width, height), std::runtime_error);
}
```

## 6. Essay questions

a) Typical buffers in our use cases are extremely sparse matrices, i.e., the vast majority of the pixels are zero. How would you exploit this to optimize the implementation?

> With regardless of how rare nonzero values exist, so I definitely assume that they are not faily enough existed

> Summation or counting operation would spend a wasteful long time in such a case. Instead, taking pairs of map to consist of position and its pixel value will be more efficient and way better in terms of runtime performance

b) What changes to the code and the API need to be made if buffer dimensions can be >= 4096 x 4096? Also, what would happen if instead of 8 bits per pixel we have 16 bits per pixel?

> MAX_WIDTH, MAX_HEIGHT are needed to be changed
- Actually, it should depend on size of data type. As an assumption that 16-bits is used, then they each will become as 2^24 (16,777,216)

> Data type should be set as *uint16_t*
> Summed type should be set as *uint64_t*
- Maximum value of summed data will become as 2^64 (2^24 (MAX_WIDTH) x 2^24 (MAX_HEIGHT) x 2^16 (BPP))
- Summed type will be then uint64_t

> And the API does not need to be modified because PixelSum class as a templete class can be instantiated in a way

c) What would change if we needed to find the maximum value inside the search window (instead of the sum or average)? You don't need to code this, but it would be interesting to know how you would approach this problem in general, and what is your initial estimate of its O-complexity.

> Finding the maximum value inside the search window would need to sort its data array so O(1) is possible when the data array is sorted.

> O-complexity will be of sorting algorithm that we use later on. For instance, O(nlog n) is possible if we first use quicksort to sort the data array and then select the maximum value. O(n^2) is possible if we first sort the data array using bubblesort and then just select the maximum value.

d) How would multi-threading change your implementation? What kinds of performance benefits would you expect?

> Since the integral image computation is absolutely the most time consuming task out of internal operations on PixelSum class, the fact that it needs to be optimized as much as we can. This is an overraching part in terms of performance benifits.

> As a way to optimize, separable logic blocks in integral image computation can be splited into multi threads. It may then impact on scanning over all pixels for non-zero and summed data computation

e) Outline shortly how the class would be implemented on a GPU (choose any relevant technology, e.g. CUDA, OpenCL). No need to code anything, just a brief overview is sufficient.

> Since each block is separably runnable in integral image computation, then we would manage to expose a second level of parallelism at this point

> A brief overview of how to implement on GPU device is defined as below:
- Take pixel data array on CPU
- Alloc integral data array on CPU and Assign to GPU 
- Bind input image as texture on GPU
- Perform integral image computation on GPU **horizontally** by several blocks and threads
- Then, perform integral image calculation on GPU **vertically** by several blocks and threads
- Unbind texture
- Copy integral data from GPU to CPU

## 7. Conclusion

Thanks for your attention!