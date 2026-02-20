#include "pixel_sum/PixelSum.h"
#include "support/TestUtility.h"
#include "support/TimeUtility.h"

TEST(PixelSum_CaseInProblemDescription_Test, GivenPixelsInsideWindow_WhenGetPixelAverage_ThenExpectedAverageIsReturned)
{
    std::vector<std::uint8_t> data { 0, 4, 0, 2, 1, 0 };
    auto pixel_sum = PixelSumU8(data.data(), 6, 1);

    auto actual = pixel_sum.getPixelAverage(0, 0, 5, 0);

    EXPECT_NEAR(actual, 1.166, 3);
}

TEST(PixelSum_CaseInProblemDescription_Test, GivenPixelsInsideWindow_WhenGetPixelAveragebySwappedPositions_ThenExpectedAverageIsReturned)
{
    std::vector<std::uint8_t> data { 0, 4, 0, 2, 1, 0 };
    auto pixel_sum = PixelSumU8(data.data(), 6, 1);

    auto actual = pixel_sum.getPixelAverage(5, 0, 0, 0);

    EXPECT_NEAR(actual, 1.166, 3);
}

TEST(PixelSum_CaseInProblemDescription_Test, GivenPixelsOverlappedWindow_WhenGetPixelAverage_ThenExpectedAverageIsReturned)
{
    std::vector<std::uint8_t> data { 1, 1, 1, 1 };
    auto pixel_sum = PixelSumU8(data.data(), 2, 2);

    auto actual = pixel_sum.getPixelAverage(1, 1, 3, 3);

    EXPECT_NEAR(actual, 0.111111, 6);
}

TEST(PixelSum_CaseInProblemDescription_Test, GivenPixelsOverlappedWindow2_WhenGetPixelAverage_ThenExpectedAverageIsReturned)
{
    std::vector<std::uint8_t> data { 1, 1, 1, 1 };
    auto pixel_sum = PixelSumU8(data.data(), 2, 2);

    auto actual = pixel_sum.getPixelAverage(0, 0, 3, 3);

    EXPECT_EQ(actual, 0.25);
}

TEST(PixelSum_CaseInProblemDescription_Test, GivenPixelsInsideWindow_WhenGetNonZeroAverage_ThenExpectedAverageIsReturned)
{
    std::vector<std::uint8_t> data { 0, 4, 0, 2, 1, 0 };
    auto pixel_sum = PixelSumU8(data.data(), 6, 1);

    auto actual_pixel_average = pixel_sum.getPixelAverage(0, 0, 5, 0);
    auto actual_nonzero_average = pixel_sum.getNonZeroAverage(0, 0, 5, 0);

    EXPECT_NEAR(actual_pixel_average, 1.166, 3);
    EXPECT_NEAR(actual_nonzero_average, 2.333, 3);
}

TEST(PixelSum_Unit_Test, GivenPixels_WhenParametrizedContruction_ThenPixelSumIsValid)
{
    std::uint32_t width = 10;
    std::uint32_t height = 10;
    std::vector<std::uint8_t> data(width * height, 128);

    auto pixel_sum = PixelSumU8(data.data(), width, height);

    EXPECT_TRUE(pixel_sum);
}

TEST(PixelSum_Unit_Test, GivenPixels_WhenCopyContruction_ThenPixelSumIsValid)
{
    std::uint32_t width = 10;
    std::uint32_t height = 10;
    std::vector<std::uint8_t> data(width * height, 128);

    auto pixel_sum = PixelSumU8(data.data(), width, height);
    auto pixel_sum_copy = PixelSumU8(pixel_sum);

    EXPECT_TRUE(pixel_sum_copy);
}

TEST(PixelSum_Unit_Test, GivenPixels_WhenAssignmentContruction_ThenPixelSumIsValid)
{
    std::uint32_t width = 10;
    std::uint32_t height = 10;
    std::vector<std::uint8_t> data(width * height, 128);

    auto pixel_sum = PixelSumU8(data.data(), width, height);
    auto pixel_sum_assignment = pixel_sum;

    EXPECT_TRUE(pixel_sum_assignment);
}

TEST(PixelSum_BoundCheck_Test, GivenZeroPixel_WhenContruction_ThenRuntimeErrorIsThrown)
{
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::vector<std::uint8_t> data(width * height);

    EXPECT_THROW(PixelSumU8(data.data(), width, height), std::runtime_error);
}

TEST(PixelSum_BoundCheck_Test, GivenValidPixels_WhenContruction_ThenNoErrorIsThrown)
{
    std::uint32_t width = 256;
    std::uint32_t height = 256;
    std::vector<std::uint8_t> data(width * height);

    EXPECT_NO_THROW(PixelSumU8(data.data(), width, height));
}

TEST(PixelSum_BoundCheck_Test, GivenInvalidPixels_WhenContruction_ThenRuntimeErrorIsThrown)
{
    std::uint32_t width = 4096 << 1;
    std::uint32_t height = 4096 << 1;
    std::vector<std::uint8_t> data(width * height);

    EXPECT_THROW(PixelSumU8(data.data(), width, height), std::runtime_error);
}

TEST(PixelSum_Smoke_Test, GivenOnePixel_WhenCallAllGetters_ThenExpectedResultsAreReturned)
{
    std::uint32_t width = 1;
    std::uint32_t height = 1;
    std::vector<std::uint8_t> data(width * height, 128);

    auto pixel_sum = PixelSumU8(data.data(), width, height);

    auto actual_pixel_average = pixel_sum.getPixelAverage(0, 0, width - 1, height - 1);
    auto actual_pixel_sum = pixel_sum.getPixelSum(0, 0, width - 1, height - 1);
    auto actual_nonzero_average = pixel_sum.getNonZeroAverage(0, 0, width - 1, height - 1);
    auto actual_nonzero_count = pixel_sum.getNonZeroCount(0, 0, width - 1, height - 1);

    EXPECT_EQ(actual_pixel_average, 128);
    EXPECT_EQ(actual_pixel_sum, 128);
    EXPECT_EQ(actual_nonzero_average, 128);
    EXPECT_EQ(actual_nonzero_count, width * height);
}

TEST(PixelSum_Smoke_Test, GivenHorizontalPixel_WhenCallAllGetters_ThenExpectedResultsAreReturned)
{
    std::uint32_t width = 4096;
    std::uint32_t height = 1;
    std::vector<std::uint8_t> data(width * height, 128);

    auto pixel_sum = PixelSumU8(data.data(), width, height);

    auto actual_pixel_average = pixel_sum.getPixelAverage(0, 0, width - 1, height - 1);
    auto actual_pixel_sum = pixel_sum.getPixelSum(0, 0, width - 1, height - 1);
    auto actual_nonzero_average = pixel_sum.getNonZeroAverage(0, 0, width - 1, height - 1);
    auto actual_nonzero_count = pixel_sum.getNonZeroCount(0, 0, width - 1, height - 1);

    EXPECT_EQ(actual_pixel_average, 128);
    EXPECT_EQ(actual_pixel_sum, 128 * width * height);
    EXPECT_EQ(actual_nonzero_average, 128);
    EXPECT_EQ(actual_nonzero_count, width * height);
}

TEST(PixelSum_Smoke_Test, GivenVerticalPixel_WhenCallAllGetters_ThenExpectedResultsAreReturned)
{
    std::uint32_t width = 1;
    std::uint32_t height = 4096;
    std::vector<std::uint8_t> data(width * height, 128);

    auto pixel_sum = PixelSumU8(data.data(), width, height);

    auto actual_pixel_average = pixel_sum.getPixelAverage(0, 0, width - 1, height - 1);
    auto actual_pixel_sum = pixel_sum.getPixelSum(0, 0, width - 1, height - 1);
    auto actual_nonzero_average = pixel_sum.getNonZeroAverage(0, 0, width - 1, height - 1);
    auto actual_nonzero_count = pixel_sum.getNonZeroCount(0, 0, width - 1, height - 1);

    EXPECT_EQ(actual_pixel_average, 128);
    EXPECT_EQ(actual_pixel_sum, 128 * width * height);
    EXPECT_EQ(actual_nonzero_average, 128);
    EXPECT_EQ(actual_nonzero_count, width * height);
}

TEST(PixelSum_Smoke_Test, GivenFullPixels_WhenCallAllGetters_ThenExpectedResultsAreReturned)
{
    std::uint32_t width = 4096;
    std::uint32_t height = 4096;
    std::vector<std::uint8_t> data(width * height, 128);

    auto pixel_sum = PixelSumU8(data.data(), width, height);

    auto actual_pixel_average = pixel_sum.getPixelAverage(0, 0, width - 1, height - 1);
    auto actual_pixel_sum = pixel_sum.getPixelSum(0, 0, width - 1, height - 1);
    auto actual_nonzero_average = pixel_sum.getNonZeroAverage(0, 0, width - 1, height - 1);
    auto actual_nonzero_count = pixel_sum.getNonZeroCount(0, 0, width - 1, height - 1);

    EXPECT_EQ(actual_pixel_average, 128);
    EXPECT_EQ(actual_pixel_sum, 128 * width * height);
    EXPECT_EQ(actual_nonzero_average, 128);
    EXPECT_EQ(actual_nonzero_count, width * height);
}

TEST(PixelSum_SearchWindow_Test, GivenPixels_WhenGetNonZeroCountWithSearchWindow_ThenExpectedNonZeroCountIsReturned)
{
    std::uint32_t width = 100;
    std::uint32_t height = 100;
    std::vector<std::uint8_t> data(width * height, 128);

    auto pixel_sum = PixelSumU8(data.data(), width, height);

    struct offset {
        int x;
        int y;
    };

    std::vector<offset> offsets { { -10, -12 }, { 2, -46 }, { 65, -74 }, { -51, -1 }, { 81, 1 }, { -37, 73 }, { 4, 19 }, { 92, 27 } };
    std::vector<uint32_t> expected_nonzero_counts { 7920, 5292, 910, 4851, 1881, 1701, 7776, 584 };

    auto expected_nonzero_counts_iter = expected_nonzero_counts.begin();
    for (auto offs : offsets) {
        auto actual_nonzero_count = pixel_sum.getNonZeroCount(offs.x, offs.y, width - 1 + offs.x, height - 1 + offs.y);

        EXPECT_EQ(actual_nonzero_count, *expected_nonzero_counts_iter++);
    }
}

int main(int argc, char* argv[])
{
    // cases in problem description
    CALL_TEST_TIMED(PixelSum_CaseInProblemDescription_Test, GivenPixelsInsideWindow_WhenGetPixelAverage_ThenExpectedAverageIsReturned);
    CALL_TEST_TIMED(PixelSum_CaseInProblemDescription_Test, GivenPixelsInsideWindow_WhenGetPixelAveragebySwappedPositions_ThenExpectedAverageIsReturned);
    CALL_TEST_TIMED(PixelSum_CaseInProblemDescription_Test, GivenPixelsOverlappedWindow_WhenGetPixelAverage_ThenExpectedAverageIsReturned);
    CALL_TEST_TIMED(PixelSum_CaseInProblemDescription_Test, GivenPixelsOverlappedWindow2_WhenGetPixelAverage_ThenExpectedAverageIsReturned);
    CALL_TEST_TIMED(PixelSum_CaseInProblemDescription_Test, GivenPixelsInsideWindow_WhenGetNonZeroAverage_ThenExpectedAverageIsReturned);

    // unit tests
    CALL_TEST_TIMED(PixelSum_Unit_Test, GivenPixels_WhenParametrizedContruction_ThenPixelSumIsValid);
    CALL_TEST_TIMED(PixelSum_Unit_Test, GivenPixels_WhenCopyContruction_ThenPixelSumIsValid);
    CALL_TEST_TIMED(PixelSum_Unit_Test, GivenPixels_WhenAssignmentContruction_ThenPixelSumIsValid);

    // bound check
    CALL_TEST_TIMED(PixelSum_BoundCheck_Test, GivenZeroPixel_WhenContruction_ThenRuntimeErrorIsThrown);
    CALL_TEST_TIMED(PixelSum_BoundCheck_Test, GivenValidPixels_WhenContruction_ThenNoErrorIsThrown);
    CALL_TEST_TIMED(PixelSum_BoundCheck_Test, GivenInvalidPixels_WhenContruction_ThenRuntimeErrorIsThrown);

    // edge case
    CALL_TEST_TIMED(PixelSum_Smoke_Test, GivenOnePixel_WhenCallAllGetters_ThenExpectedResultsAreReturned);
    CALL_TEST_TIMED(PixelSum_Smoke_Test, GivenHorizontalPixel_WhenCallAllGetters_ThenExpectedResultsAreReturned);
    CALL_TEST_TIMED(PixelSum_Smoke_Test, GivenVerticalPixel_WhenCallAllGetters_ThenExpectedResultsAreReturned);
    CALL_TEST_TIMED(PixelSum_Smoke_Test, GivenFullPixels_WhenCallAllGetters_ThenExpectedResultsAreReturned); // it may take a fairly long time due to huge dimension

    // search window test
    CALL_TEST_TIMED(PixelSum_SearchWindow_Test, GivenPixels_WhenGetNonZeroCountWithSearchWindow_ThenExpectedNonZeroCountIsReturned);

    return 0;
}