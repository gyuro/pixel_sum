cmake_minimum_required(VERSION 3.20)

project(
    PixelSum
    VERSION 1.1.0
    LANGUAGES CXX)

include(GNUInstallDirs)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PIXEL_SUM_ENABLE_SANITIZERS "Enable ASAN/UBSAN in Debug" OFF)

if(PIXEL_SUM_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_library(PixelSumLib PixelSum.cpp PixelSum.h TestUtility.h TimeUtility.h Common.h)
target_include_directories(PixelSumLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(PixelSumLib PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(PixelSumTest PixelSumTest.cpp)
target_link_libraries(PixelSumTest PRIVATE PixelSumLib)

if(BUILD_TESTING)
    add_test(NAME PixelSumTest COMMAND PixelSumTest)
endif()

# ---- Quality targets ----
find_program(CLANG_FORMAT clang-format)
find_program(CLANG_TIDY clang-tidy)
find_program(CPPCHECK cppcheck)

set(PIXEL_SUM_ALL_CPP
    ${CMAKE_SOURCE_DIR}/PixelSum.cpp
    ${CMAKE_SOURCE_DIR}/PixelSum.h
    ${CMAKE_SOURCE_DIR}/PixelSumTest.cpp
    ${CMAKE_SOURCE_DIR}/TestUtility.h
    ${CMAKE_SOURCE_DIR}/TimeUtility.h
    ${CMAKE_SOURCE_DIR}/Common.h)

if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i -style=file ${PIXEL_SUM_ALL_CPP}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format"
        VERBATIM)

    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT} --dry-run --Werror -style=file ${PIXEL_SUM_ALL_CPP}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Checking clang-format"
        VERBATIM)
endif()

if(CLANG_TIDY)
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/PixelSum.cpp ${CMAKE_SOURCE_DIR}/PixelSumTest.cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy"
        VERBATIM)
endif()

if(CPPCHECK)
    set(PIXEL_SUM_CPPCHECK_ARGS
        --enable=warning,style,performance,portability
        --std=c++17
        --language=c++
        --inline-suppr)

    if(EXISTS ${CMAKE_SOURCE_DIR}/.cppcheck-suppressions)
        list(APPEND PIXEL_SUM_CPPCHECK_ARGS --suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck-suppressions)
    endif()

    add_custom_target(cppcheck
        COMMAND ${CPPCHECK} ${PIXEL_SUM_CPPCHECK_ARGS} ${CMAKE_SOURCE_DIR}/PixelSum.cpp ${CMAKE_SOURCE_DIR}/PixelSumTest.cpp
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running cppcheck"
        VERBATIM)
endif()

install(TARGETS PixelSumLib PixelSumTest
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES PixelSum.h Common.h TestUtility.h TimeUtility.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/PixelSum)

install(FILES README.md DESTINATION ${CMAKE_INSTALL_DATADIR}/PixelSum)

set(CPACK_PACKAGE_NAME "pixel_sum")
set(CPACK_PACKAGE_VENDOR "pixel_sum")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git/;\\.swp$;\\.DS_Store")
include(CPack)
