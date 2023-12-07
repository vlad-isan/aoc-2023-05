cmake_minimum_required(VERSION 3.27)
project(aoc-template)

set(CMAKE_CXX_STANDARD 23)

set(VCPKG_MANIFEST_MODE ON)

find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        puzzle.cpp
        puzzle.h)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        fmt::fmt
)

set(TEST_TARGET_NAME ${PROJECT_NAME}-test)

add_executable(${TEST_TARGET_NAME} test.cpp
        test.cpp
        puzzle.cpp
        puzzle.h)

target_link_libraries(${TEST_TARGET_NAME}
        PUBLIC
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
        fmt::fmt
)