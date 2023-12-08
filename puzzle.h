//
// Created by Vlad Isan on 05/12/2023.
//

#ifndef PUZZLE_H
#define PUZZLE_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "fmt/core.h"

enum class MapType {
    SEED,
    SOIL,
    FERTILIZER,
    WATER,
    LIGHT,
    TEMPERATURE,
    HUMIDITY,
    LOCATION
};

const std::pmr::unordered_map<std::string, MapType> map_type_map = {
    {"seed-to-soil map:", MapType::SOIL},
    {"soil-to-fertilizer map:", MapType::FERTILIZER},
    {"fertilizer-to-water map:", MapType::WATER},
    {"water-to-light map:", MapType::LIGHT},
    {"light-to-temperature map:", MapType::TEMPERATURE},
    {"temperature-to-humidity map:", MapType::HUMIDITY},
    {"humidity-to-location map:", MapType::LOCATION}
};

const std::pmr::unordered_map<MapType, std::string> map_type_string = {
    {MapType::SEED, "seed"},
    {MapType::SOIL, "soil"},
    {MapType::FERTILIZER, "fertilizer"},
    {MapType::WATER, "water"},
    {MapType::LIGHT, "light"},
    {MapType::TEMPERATURE, "temperature"},
    {MapType::HUMIDITY, "humidity"},
    {MapType::LOCATION, "location"}
};

using Range = std::pair<uint64_t, uint64_t>;

struct Map {
    Range range;
    Range source_range;
};

struct Almanac {
   std::map<MapType, std::vector<Map>> maps_types;
};

int puzzle_sample_1(const std::string &base_file_path);

int puzzle_sample_2(const std::string &base_file_path);

int puzzle_1(const std::string &base_file_path);

int puzzle_2(const std::string &base_file_path);

int do_puzzle_1(std::ifstream &file);

int do_puzzle_2(std::ifstream &file);

std::vector<Map> parse_seeds(const std::string &line);
std::vector<Map> parse_seeds_ranges(const std::string &line);

void parse_list(std::ifstream &file, Almanac &almanac);

int get_lowest_location(Almanac &almanac, uint64_t source_value, MapType map_type);
int get_lowest_location_ranges(Almanac &almanac, const Range &source_range, MapType map_type);

#endif //PUZZLE_H
