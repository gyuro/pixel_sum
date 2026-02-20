#pragma once

#include "support/Common.h"

#include <chrono>
#include <iostream>

#define TIME_START() auto start = std::chrono::steady_clock::now();
#define TIME_END() auto end = std::chrono::steady_clock::now();
#define TIME_ELAPSED()                                                                \
    std::cout << " It took " YELLOW                                                   \
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)    \
                     .count() / 1000000.f                                              \
              << RESET " ms" << std::endl;
