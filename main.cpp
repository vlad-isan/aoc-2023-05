#include "fmt/core.h"
#include "puzzle.h"

int main() {
    static constexpr const char* FILE_PATH_ENV = "AOC_FILE_PATH";
    char* base_file_path = std::getenv(FILE_PATH_ENV);

    if (base_file_path == nullptr) {
        fmt::println("Set the AOC_FILE_PATH environment variable for the path where to read files from, you muppet!");
        return EXIT_FAILURE;
    }

    fmt::println("Puzzle 1 result is {}", puzzle_1(base_file_path));
    fmt::println("Puzzle 2 result is {}", puzzle_2(base_file_path));

    return 0;
}
