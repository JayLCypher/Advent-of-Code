
#include <helpers/utility.h>
#include <helpers/file.h>
#define _need_mallocator
#include <helpers/allocator.h>
#include <stdio.h>
#include <ctype.h>

bool all_increase(const size_t count, const size_t items SCR(count), size_t *skip_index);
bool all_decrease(const size_t count, const size_t items SCR(count), size_t *skip_index);
bool all_lesser(const size_t count, const size_t items SCR(count), size_t *skip_index);
bool all_greater(const size_t count, const size_t items SCR(count), size_t *skip_index);

inline
bool all_increase(const size_t count, const size_t items SCR(count), size_t *skip_index)
{
	assert(skip_index);
	for (size_t i = 1; i != count; ++i) {
		if (*skip_index == i - 1) { continue; }
		if (!(items[i - 1] < items[i + (*skip_index == i)])) {
			if (*skip_index == count+1) { *skip_index = i; continue; }
			return false;
		}
	}
	return true;
}

inline
bool all_greater(const size_t count, const size_t items SCR(count), size_t *skip_index)
{
	assert(skip_index);
	for (size_t i = 1; i != count; ++i) {
		if (*skip_index == i - 1) { continue; }
		if (!(items[i - i] + 3 >= items[i + (*skip_index == i)])) {
			return false;
		}
	}
	return true;
}

inline
bool all_decrease(const size_t count, const size_t items SCR(count), size_t *skip_index)
{
	assert(skip_index);
	for (size_t i = 1; i != count; ++i) {
		if (*skip_index == i - 1) { continue; }
		if (!(items[i - 1] > items[i + (*skip_index == i)])) {
			if (*skip_index == count+1) { *skip_index = i; continue; }
			return false;
		}
	}
	return true;
}

inline
bool all_lesser(const size_t count, const size_t items SCR(count), size_t *skip_index)
{
	assert(skip_index);
	for (size_t i = 1; i != count; ++i) {
		if (*skip_index == i - 1) { continue; }
		if (!(items[i + (*skip_index == i)] + 3 >= items[i - 1])) {
			return false;
		}
	}
	return true;
}

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv SCR(argc)) {
	putss("Advent of Code: Day 2");

	if (argc < 2) {
		errputs("Please provide input file...");
		return 1;
	}

	const char *filename = argv[1];
	size_t data_size = 0;
	char *const data = file_dump_allocator(filename, &data_size, &mallocator);
	// Data is in format NUMBER spaces NUMBER newline
	putss(data);

	size_t safe_count = 0;

	// The levels are either all increasing or all decreasing.
	// Any two adjacent levels differ by at least one and at most three.

	char *data_iter = data;
	static constexpr size_t item_count = 20;
	size_t items[item_count] = {0};
	while (data_iter && *data_iter != '\0') {
		while (isspace(*data_iter)) { ++data_iter; }
		size_t count = 0;
		while (count != item_count) {
			items[count++] = strtoul(data_iter, &data_iter, 10);
			if (*data_iter == '\n') { ++data_iter; break; }
		}
		// size_t one = strtoul(data_iter, &data_iter, 10);
		// size_t two = strtoul(data_iter, &data_iter, 10);
		// size_t three = strtoul(data_iter, &data_iter, 10);
		// size_t four = strtoul(data_iter, &data_iter, 10);
		// size_t five = strtoul(data_iter, &data_iter, 10);

		size_t skip_index = count+1;
		// All increasing
		if (all_increase(count, items, &skip_index)) {
			if (all_greater(count, items, &skip_index)) {
				++safe_count;
				continue;
			}
		}
		skip_index = count+1;
		// All decreasing
		if (all_decrease(count, items, &skip_index)) {
			if (all_lesser(count, items, &skip_index)) {
				++safe_count;
				continue;
			}
		}
	}

	printfs("%zu\n", safe_count);

	deallocate(&mallocator, data);
	return 0;
}
