//
// Created by Vlad Isan on 05/12/2023.
//

#ifndef PUZZLE_H
#define PUZZLE_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include "fmt/core.h"

int puzzle_sample_1(const std::string &base_file_path);

int puzzle_sample_2(const std::string &base_file_path);

int puzzle_1(const std::string &base_file_path);

int puzzle_2(const std::string &base_file_path);

int do_puzzle_1(std::ifstream &file);

int do_puzzle_2(std::ifstream &file);

#endif //PUZZLE_H
