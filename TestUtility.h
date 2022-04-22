#pragma once

#include "Common.h"
#include <iostream>
#include <cmath>

/* Comparison macros */
#define EXPECT_NEAR(actual, expected, precision)                                                  \
    do {                                                                                          \
        auto actual_ = actual;                                                                    \
        auto expected_ = expected;                                                                \
        double epsilon = precision == 0.0 ? 0.0 : std::pow(0.1, precision);                       \
        if (std::abs(static_cast<double>(actual_) - static_cast<double>(expected_)) <= epsilon) { \
            std::cout << GREEN " Success:" RESET;                                                 \
        } else {                                                                                  \
            std::cerr << RED " Failure:" RESET;                                                   \
        }                                                                                         \
        std::cout << " actual " << actual_ << " which expects " << expected_ << " ("              \
                  << epsilon << " eps)" << std::endl;                                             \
    } while (0)

#define EXPECT_EQ(actual, expected) EXPECT_NEAR(actual, expected, 0.0)

#define EXPECT_GE(val1, val2)                                                \
    do {                                                                     \
        auto val1_ = val1;                                                   \
        auto val2_ = val2;                                                   \
        if (val1_ >= val2_) {                                                \
            std::cout << GREEN " Success:" RESET;                            \
        } else {                                                             \
            std::cerr << RED " Failure:" RESET;                              \
        }                                                                    \
        std::cout << " val1 " << val1_ << " >= val2 " << val2_ << std::endl; \
    } while (0)

#define EXPECT_GT(val1, val2)                                               \
    do {                                                                    \
        auto val1_ = val1;                                                  \
        auto val2_ = val2;                                                  \
        if (val1_ >= val2_) {                                               \
            std::cout << GREEN " Success:" RESET;                           \
        } else {                                                            \
            std::cerr << RED " Failure:" RESET;                             \
        }                                                                   \
        std::cout << " val1 " << val1_ << " > val2 " << val2_ << std::endl; \
    } while (0)

#define EXPECT_TRUE(val)                         \
    do {                                         \
        auto val_ = val;                         \
        if (val_) {                              \
            std::cout << GREEN " Success" RESET; \
        } else {                                 \
            std::cerr << RED " Failure" RESET;   \
        }                                        \
        std::cout << std::endl;                  \
    } while (0)

#define EXPECT_NO_THROW(method)                                                             \
    do {                                                                                    \
        bool thrown = false;                                                                \
        try {                                                                               \
            method;                                                                         \
        } catch (...) {                                                                     \
            thrown = true;                                                                  \
        }                                                                                   \
        if (!thrown) {                                                                      \
            std::cout << GREEN " Success: " RESET << "No exception is thrown" << std::endl; \
        } else {                                                                            \
            std::cerr << RED " Failure" RESET << std::endl;                                 \
        }                                                                                   \
    } while (0)

#define EXPECT_THROW(method, exception)                                 \
    do {                                                                \
        std::string what {};                                            \
        try {                                                           \
            method;                                                     \
        } catch (exception & e) {                                       \
            what = e.what();                                            \
        }                                                               \
        if (!what.empty()) {                                            \
            std::cout << GREEN " Success: " RESET << what << std::endl; \
        } else {                                                        \
            std::cerr << RED " Failure" RESET << std::endl;             \
        }                                                               \
    } while (0)

/* Test stuffs and call itself macros */
class ITextFixture {
public:
    explicit ITextFixture(const std::string& test_case_name,
        const std::string& test_name)
        : test_case_name_(test_case_name)
        , test_name_(test_name)
    {
    }

protected:
    virtual void TestBody() = 0;
    std::string test_case_name_;
    std::string test_name_;
};

#define TEST_NAME_(test_case_name, test_name) \
    test_case_name##_##test_name##_Test
#define TEST_INSTANCE_NAME_(test_case_name, test_name) \
    test_case_name##_##test_name

#define TEST(test_case_name, test_name)                                              \
    class TEST_NAME_(test_case_name, test_name)                                      \
        : public ITextFixture {                                                      \
    public:                                                                          \
        using ITextFixture::ITextFixture;                                            \
        void TestBody() override;                                                    \
        void Test() { TestBody(); }                                                  \
        void TestTimed()                                                             \
        {                                                                            \
            TIME_START()                                                             \
            Test();                                                                  \
            TIME_END()                                                               \
            TIME_ELAPSED()                                                           \
        }                                                                            \
        void PreTest()                                                               \
        {                                                                            \
            std::cout << test_case_name_ << "_" << test_name_ << RESET << std::endl; \
        }                                                                            \
        void PostTest() { std::cout << std::endl; }                                  \
    };                                                                               \
    TEST_NAME_(test_case_name, test_name)                                            \
    TEST_INSTANCE_NAME_(test_case_name, test_name)                                   \
    (#test_case_name, #test_name);                                                   \
    void TEST_NAME_(test_case_name, test_name)::TestBody()

#define CALL_TEST(test_case_name, test_name)                  \
    TEST_INSTANCE_NAME_(test_case_name, test_name).PreTest(); \
    TEST_INSTANCE_NAME_(test_case_name, test_name).Test();    \
    TEST_INSTANCE_NAME_(test_case_name, test_name).PostTest();

#define CALL_TEST_TIMED(test_case_name, test_name)              \
    TEST_INSTANCE_NAME_(test_case_name, test_name).PreTest();   \
    TEST_INSTANCE_NAME_(test_case_name, test_name).TestTimed(); \
    TEST_INSTANCE_NAME_(test_case_name, test_name).PostTest();
