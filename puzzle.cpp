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

    uint32_t lowest_location = UINT32_MAX;

    for (const auto &seed: almanac.maps_types[(MapType)0]) {
        const uint32_t location = get_lowest_location(almanac, seed.range.first, MapType::SOIL);

        if (location < lowest_location) {
            lowest_location = location;
        }
    }

    return lowest_location;
}

int do_puzzle_2(std::ifstream &file) {
    std::string line;

    while (std::getline(file, line)) {
        fmt::println("{}", line);
    }

    return 0;
}

std::vector<Map> parse_seeds(const std::string &line) {
    std::vector<Map> seeds;
    std::istringstream stream{line};
    std::string ignore;
    uint32_t range_start;
    stream >> ignore;

    while (stream >> range_start) {
        seeds.emplace_back(Map{.range = {range_start, range_start}});
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

            fmt::println("parsing list for line {}", line);
            continue;
        }

        std::istringstream iss{line};

        uint32_t dest_start;
        uint32_t source_start;
        uint32_t len;

        iss >> dest_start >> source_start >> len;
        fmt::println("{} {} {}", dest_start, source_start, len);

        almanac.maps_types[current_map_type].emplace_back(Map{
            .range = {dest_start, dest_start + len},
            .source_range = {source_start, source_start + len}
        });
    }

    // current map's now parsed
    // we should match the sources & destination
    // for (auto &source: sources) {
    //     std::vector<Range> existing_ranges{destinations.size()};
    //     std::vector<Range> new_ranges;
    //
    //     for (auto &dest: destinations) {
    //         existing_ranges.emplace_back(dest->source_range);
    //     }
    //
    //     std::sort(existing_ranges.begin(), existing_ranges.end());
    //     uint32_t end = source->range.first;
    //
    //     for (const auto &existing_range: existing_ranges) {
    //         if (end < existing_range.first) {
    //             new_ranges.emplace_back(end, existing_range.first);
    //         }
    //
    //         end = std::max(end, existing_range.second);
    //     }
    //
    //     if (end < source->range.second) {
    //         new_ranges.emplace_back(end, source->range.second);
    //     }
    //
    //     auto it = std::ranges::find_if(destinations, [&](auto &dest) {
    //         return source->range.first >= dest->source_range.first && source->range.second <= dest->source_range.second;
    //     });
    //
    //     if (it != destinations.end()) {
    //         source->destination = *it;
    //     }
    //     else {
    //         // source->destination = new Map{
    //         //     .type = current_map_type,
    //         //     .range = source->range,
    //         //     .source_range = source->range
    //         // };
    //         //
    //         // destinations.emplace_back(source->destination);
    //     }
    // }
    //
    // // drill down into destinations
    // if (!destinations.empty() && !file.eof()) {
    //     parse_list(file, destinations);
    // }
}

int get_lowest_location(Almanac &almanac, uint32_t source_value, MapType map_type) {
    auto it = std::ranges::find_if(almanac.maps_types.at(map_type), [&](auto &dest) {
        return source_value >= dest.source_range.first && source_value <= dest.source_range.second;
    });

    const Map &map = (it != almanac.maps_types.at(map_type).end()) ? *it : Map{.range = {source_value, source_value}, .source_range = {source_value, source_value}};


    uint32_t value = map.range.first + (source_value - map.source_range.first);

    return map_type == MapType::LOCATION ? value : get_lowest_location(almanac, value, (MapType)((int)map_type + 1));
}
