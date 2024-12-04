#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

static constexpr const char *example[] = {
	"seeds: 79 14 55 13",
	"",
	"seed-to-soil map:",
	"50 98 2",
	"52 50 48",
	"",
	"soil-to-fertilizer map:",
	"0 15 37",
	"37 52 2",
	"39 0 15",
	"",
	"fertilizer-to-water map:",
	"49 53 8",
	"0 11 42",
	"42 0 7",
	"57 7 4",
	"",
	"water-to-light map:",
	"88 18 7",
	"18 25 70",
	"",
	"light-to-temperature map:",
	"45 77 23",
	"81 45 19",
	"68 64 13",
	"",
	"temperature-to-humidity map:",
	"0 69 1",
	"1 0 69",
	"",
	"humidity-to-location map:",
	"60 56 37",
	"56 93 4",
};

constexpr bool print_input = false;
constexpr bool do_example = true;
constexpr size_t example_location[] = {35, 46};
constexpr auto do_part = 2;
constexpr const char *const input_file_name = "input.txt";

namespace {

size_t part1(const std::vector<std::string> &input) {
	// The maps are as follows:
	// DST SRC RANGE
	// <SRC, SRC+RANGE> = <MIN, MAX>
	// <DST, DST+RANGE> = <MIN, MAX>
	// What to save.
	// SRC <- starting point
	// SRC + Range <- ending point
	// if (seed >= SRC && seed <= SRC + RANGE) { seed = DST + (seed - SRC); }
	size_t location = 0;
	auto seeds_vec = std::vector<size_t>();
	auto maps_vec =
		std::vector<std::unordered_map<size_t, std::pair<size_t, size_t>>>();
	auto map = std::unordered_map<size_t, std::pair<size_t, size_t>>();
	for (const auto &s : input) {
		auto line = std::istringstream{s};
		auto word = std::string{};
		line >> word;
		if (word.empty()) {
			maps_vec.push_back(map);  // Push it in the vector of maps.
			map = std::unordered_map<size_t, std::pair<size_t, size_t>>(); // New map.
			continue;
		}
		if (word == "seeds:") {
			// Build the vector of seeds
			std::cout << word << " ";
			while (line >> word) {
				auto seed_num = std::strtoull(word.c_str(), nullptr, 10);
				seeds_vec.push_back(seed_num);
				std::cout << seed_num << " ";
			}
			std::cout << '\n';
		} else {
			auto dest_str = std::string{word};
			auto src_str = std::string{};
			auto range_str = std::string{};
			line >> src_str;
			if (src_str == "map:") {
				std::cout << dest_str << ":" << '\n';
			} else {
				line >> range_str;
				auto dest = std::strtoull(dest_str.c_str(), nullptr, 10);
				auto src = std::strtoull(src_str.c_str(), nullptr, 10);
				auto range = std::strtoull(range_str.c_str(), nullptr, 10);
				// Create a row of src -> dst.
				// std::cout << "D: " << dest << "\t| S: " << src << "\t| R: "
				// << range << std::endl; std::cout << "Inserting { " << src <<
				// ", < " << dest << ", " << range << "> };" << std::endl;
				map.emplace(src, std::make_pair(dest, range));
			}
		}
	}
	maps_vec.push_back(map);  // Push it in the vector of maps.
	std::cout << "Maps amount: " << maps_vec.size() << '\n';
	location = seeds_vec.front();
	for (auto seed : seeds_vec) {
		auto cur = seed;
		for (const auto &local_map : maps_vec) {
			for (auto e : local_map) {
				// std::cout << "Checking if " << cur << " in range [" <<
				// e.first << ", " << e.first + e.second.second << "]" <<
				// std::endl;
				if (cur >= e.first && cur < e.first + e.second.second) {
					// std::cout << "Old cur diff: " << cur - e.first << " |
					// Cur: " << cur;
					cur = e.second.first + (cur - e.first);
					// std::cout << " -> " << cur << std::endl;
					break;
				}
			}
		}
		// std::cout << "Is " << cur << " < " << location << " ? " << (cur <
		// location) << std::endl;
		location = cur < location ? cur : location;
	}
	return location;
}

size_t part2(const std::vector<std::string> &input) {
	std::cout << "Starting Part 2!" << '\n';
	size_t location = 0;
	auto seeds_vec = std::vector<std::vector<size_t>>();
	auto maps_vec = std::vector<std::vector<std::vector<size_t>>>();
	auto map_name_vec = std::vector<std::string>();
	auto map = std::vector<std::vector<size_t>>();
	for (const auto &s : input) {
		auto line = std::istringstream{s};
		auto word = std::string{};
		line >> word;
		if (word.empty()) {
			maps_vec.push_back(map);  // Push it in the vector of maps.
			map = std::vector<std:vector<size_t>>(); // New Map
			continue;
		}
		if (word == "seeds:") {
			// Build the vector of seeds
			std::cout << word << " ";
			while (line >> word) {
				auto seed_num = std::strtoull(word.c_str(), nullptr, 10);
				auto seed_range_str = std::string();
				line >> seed_range_str;
				auto seed_range =
					std::strtoull(seed_range_str.c_str(), nullptr, 10);
				std::cout << seed_num << " " << seed_range << " ";
				auto v = std::vector<size_t>();
				for (size_t i = seed_num; i < seed_num + seed_range; ++i) {
					v.push_back(i);
				}
				seeds_vec.push_back(v);
			}
			std::cout << '\n';
		} else {
			auto dest_str = std::string{word};
			auto src_str = std::string{};
			auto range_str = std::string{};
			line >> src_str;
			if (src_str == "map:") {
				std::cout << "Building map " << dest_str << " ...";
				map_name_vec.push_back(dest_str);
			} else {
				line >> range_str;
				auto dest = std::strtoull(dest_str.c_str(), nullptr, 10);
				auto src = std::strtoull(src_str.c_str(), nullptr, 10);
				auto range = std::strtoull(range_str.c_str(), nullptr, 10);
				// Create a row of src -> dst.
				std::cout << "D: " << dest << "\t| S: " << src
						  << "\t| R: " << range << '\n';
				std::cout << "Inserting { " << src << ", < " << dest << ", "
						  << range << "> };" << '\n';
				auto vec = std::vector<size_t>();
				vec.reserve(range);
				for (size_t i = src; i < src + range; ++i) {
					vec.push_back(dest + i);
				}
				map.push_back(vec);
				std::cout << " Section Finished." << '\n';
			}
		}
	}

	// maps_vec.push_back(map); // Push it in the vector of maps.
	std::cout << "Maps amount: " << maps_vec.size() << '\n';
	// Build a vector of all seeds
	std::cout << "Building vector of numbers" << '\n';
	auto results = std::vector<size_t>();
	for (const auto &seed : seeds_vec) {
		std::cout << "Seeds: [" << seed.front() << ".." << "]" << '\n';
		std::cout << "Checking all values through the maps." << '\n';
		size_t i = 0;
		for (const auto &local_map : maps_vec) {
			std::cout << map_name_vec[i] << ": ";
			for (const auto &seed_i : seed) {
				std::cout << "[" << seed_i << " -> ";
				seed_i = local_map.contains(seed_i) ? local_map.at(seed_i) : seed_i;
				std::cout << seed_i << "] ";
			}
			std::cout << '\n';
			++i;
		}
		std::cout << "Done with maps, checking end results." << '\n';
		location = seeds.front();
		for (auto seed_i : seeds) {
			location = location > seed_i ? seed_i : location;
		}
		results.push_back(location);
	}
	std::cout << "res: ";
	for (auto &res : results) {
		std::cout << res << " ";
		location = location > res ? res : location;
	}
	std::cout << '\n';
	return location;
}
} // namespace


int main() {
	std::cout << "Advent of Code Day 5!\n" << '\n';

	// Set up input
	auto input = std::vector<std::string>();
	if (do_example) {
		for (const auto &s : example) {
			input.emplace_back(s);
		}
	} else {
		auto file_input = std::ifstream{input_file_name};
		if (!file_input) {
			std::cerr << "ERROR: Cannot open file " << input_file_name
					  << "for reading\n"
					  << '\n';
			return -1;
		}
		auto file_line = std::string{};
		while (std::getline(file_input, file_line)) {
			input.push_back(file_line);
		}
		input.emplace_back("");
	}
	if (print_input) {
		for (const auto &s : input) {
			std::cout << s << '\n';
		}
	}

	size_t location = 0;
	if (do_part == 1) {
		location = part1(input);
	}
	else if (do_part == 2) {
		location = part2(input);
	}

	if (do_example) {
		auto example_var = example_location[do_part == 2];
		std::cout << "Location is " << location << " and should be " << example_var << '\n';
		assert(location == example_var);
	} else {
		std::cout << "Location is: " << location << '\n';
	}
	return 0;
}
