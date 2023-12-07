//
// Created by Vlad Isan on 05/12/2023.
//

#include <gtest/gtest.h>
#include "puzzle.h"

static constexpr const char* FILE_PATH_ENV = "AOC_FILE_PATH";
char *base_file_path = std::getenv(FILE_PATH_ENV);

std::string get_base_file_path() {
    if (base_file_path == nullptr) {
        fmt::println("Set the AOC_FILE_PATH environment variable for the path where to read files from, you muppet!");
        return "";
    }

    return std::string{base_file_path};
}

TEST(AOC, PUZZLE_SAMPLE_1) {
    fmt::println("{}", base_file_path);
    EXPECT_EQ(puzzle_sample_1(get_base_file_path()), 0);
}

TEST(AOC, PUZZLE_SAMPLE_2) {
    fmt::println("{}", base_file_path);
    EXPECT_EQ(puzzle_sample_2(get_base_file_path()), 0);
}

TEST(AOC, PUZZLE_1) {
    fmt::println("{}", base_file_path);
    EXPECT_EQ(puzzle_1(get_base_file_path()), 0);
}

TEST(AOC, PUZZLE_2) {
    EXPECT_EQ(puzzle_2(get_base_file_path()), 0);
}

