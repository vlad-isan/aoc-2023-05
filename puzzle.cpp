//
// Created by Vlad Isan on 05/12/2023.
//

#include "puzzle.h"

int puzzle_sample_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

int puzzle_sample_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-sample-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

int puzzle_1(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-1.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_1(file);
}

int puzzle_2(const std::string &base_file_path) {
    std::string file_path = fmt::format("{}/{}", base_file_path, "puzzle-input-2.txt");

    std::ifstream file(file_path);

    if (!file.is_open()) {
        fmt::println("{}", file_path);
        fmt::println("Something happened while opening the file. EAT DEEZNUTS!!!");
        fmt::println("Error {}", strerror(errno));

        return 0;
    }

    return do_puzzle_2(file);
}

int do_puzzle_1(std::ifstream &file) {
    std::string line;

    std::getline(file, line);

    std::vector<Map> seeds = parse_seeds(line);

    Almanac almanac;
    almanac.maps_types[MapType::SEED] = seeds;

    parse_list(file, almanac);

    uint64_t lowest_location = UINT64_MAX;

    for (const auto &seed: almanac.maps_types[(MapType)0]) {
        const uint64_t location = get_lowest_location(almanac, seed.range.first, MapType::SOIL);

        if (location < lowest_location) {
            lowest_location = location;
        }
    }

    return lowest_location;
}

int do_puzzle_2(std::ifstream &file) {
    std::string line;

    std::getline(file, line);

    std::vector<Map> seeds = parse_seeds_ranges(line);

    Almanac almanac;
    almanac.maps_types[MapType::SEED] = seeds;

    parse_list(file, almanac);

    uint64_t lowest_location = UINT64_MAX;

    for (const auto &seed: almanac.maps_types[MapType::SEED]) {
        const uint64_t location = get_lowest_location_ranges(almanac, seed.range, MapType::SOIL);

        if (location < lowest_location) {
            lowest_location = location;
        }
    }

    return lowest_location;
}

std::vector<Map> parse_seeds(const std::string &line) {
    std::vector<Map> seeds;
    std::istringstream stream{line};
    std::string ignore;
    uint64_t range_start;
    stream >> ignore;

    while (stream >> range_start) {
        seeds.emplace_back(Map{.range = {range_start, range_start}});
    }

    return seeds;
}

std::vector<Map> parse_seeds_ranges(const std::string &line) {
    std::vector<Map> seeds;
    std::istringstream stream{line};
    std::string ignore;
    uint64_t range_start;
    uint64_t range_len;
    stream >> ignore;

    while (stream >> range_start >> range_len) {
        seeds.emplace_back(Map{.range = {range_start, range_start + range_len}});
    }

    return seeds;
}

void parse_list(std::ifstream &file, Almanac &almanac) {
    std::string line;

    MapType current_map_type;

    while (std::getline(file, line)) {
        if (line.empty()) {
            std::getline(file, line); // This gets the map title

            current_map_type = map_type_map.at(line);

            almanac.maps_types[current_map_type] = std::vector<Map>{};
            continue;
        }

        std::istringstream iss{line};

        uint64_t dest_start;
        uint64_t source_start;
        uint64_t len;

        iss >> dest_start >> source_start >> len;

        almanac.maps_types[current_map_type].emplace_back(Map{
            .range = {dest_start, dest_start + len},
            .source_range = {source_start, source_start + len}
        });
    }
}

int get_lowest_location(Almanac &almanac, uint64_t source_value, MapType map_type) {
    auto it = std::ranges::find_if(almanac.maps_types.at(map_type), [&](auto &dest) {
        return source_value >= dest.source_range.first && source_value <= dest.source_range.second;
    });

    const Map &map = (it != almanac.maps_types.at(map_type).end())
                         ? *it
                         : Map{.range = {source_value, source_value}, .source_range = {source_value, source_value}};


    uint64_t value = map.range.first + (source_value - map.source_range.first);

    return map_type == MapType::LOCATION ? value : get_lowest_location(almanac, value, (MapType)((int)map_type + 1));
}

int get_lowest_location_ranges(Almanac &almanac, const Range &source_range, MapType map_type) {
    std::vector<Map> &destinations = almanac.maps_types.at(map_type);
    std::vector<Range> existing_ranges;
    std::vector<Map> new_destinations;

    existing_ranges.reserve(destinations.size());
    for (auto &dest: destinations) {
        existing_ranges.emplace_back(dest.source_range);
    }

    std::ranges::sort(existing_ranges);
    uint64_t it = source_range.first;
    bool is_covered = false; // Flag to check if source_range is covered by existing ranges

    for (const auto &existing_range: existing_ranges) {
        // If the current point (it) is already beyond the source range, break the loop
        if (it >= source_range.second) {
            is_covered = true;
            break;
        }

        // Check if there is a gap before the existing_range and the source_range is not yet fully covered
        if (it < existing_range.first && !is_covered) {
            new_destinations.emplace_back(Map{
                .range = {it, std::min(source_range.second, existing_range.first)},
                .source_range = {it, std::min(source_range.second, existing_range.first)}
            });
        }

        // Update it to the end of the current existing_range if it extends beyond it
        if (existing_range.second > it) {
            it = existing_range.second;
        }
    }

    // Add the remaining part of source_range only if it was not covered by any existing range
    if (!is_covered && it < source_range.second) {
        new_destinations.emplace_back(Map{
            .range = {it, source_range.second},
            .source_range = {it, source_range.second}
        });
    }

    destinations.insert_range(destinations.end(), new_destinations);

    uint64_t lowest_value = UINT64_MAX;

    for (const auto &dest: destinations | std::views::filter([&](auto &d) {
        return source_range.first < d.source_range.second && d.source_range.first < source_range.second;
    })) {
        uint64_t value = 0;

        if (map_type == MapType::LOCATION) {
            if (source_range.first <= dest.source_range.first) {
                value = dest.range.first;
            }
            else {
                value = dest.range.first + source_range.first - dest.source_range.first;
            }
        }
        else {
            std::pair<uint64_t, uint64_t> new_range;

            new_range.first = dest.range.first + (source_range.first <= dest.source_range.first
                                                      ? 0
                                                      : source_range.first - dest.source_range.first);
            new_range.second = dest.range.second - (source_range.second >= dest.source_range.second
                                                        ? 0
                                                        : dest.source_range.second - source_range.second);

            value = get_lowest_location_ranges(almanac, new_range,
                                               static_cast<MapType>(static_cast<int>(map_type) + 1));
        }

        if (value < lowest_value) {
            lowest_value = value;
        }
    }

    return lowest_value;
}
