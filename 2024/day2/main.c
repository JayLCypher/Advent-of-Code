
#include <helpers/utility.h>
#include <helpers/file.h>
#define _need_mallocator
#include <helpers/allocator.h>
#include <stdio.h>
#include <ctype.h>

[[maybe_unused]]
static
bool check_numbers_part1(const size_t count, const size_t items SCR(count))
{
	assert(count > 2);
	static constexpr size_t limit = 3+1;
	// All increasing within limits
	if (items[0] < items[1]) {
		// putss("Increase");
		for (size_t i = 1; i != count; ++i) {
			if (!(items[i - 1] < items[i] && items[i - 1] + limit > items[i])) { return false; }
		}
		return true;
	}
	// All decreasing within limits
	else if (items[0] > items[1]) {
		// putss("Decrease");
		for (size_t i = 1; i != count; ++i) {
			if (!(items[i - 1] > items[i] && items[i - 1] < items[i] + limit)) { return false; }
		}
		return true;
	}
	return false;
}

[[maybe_unused]]
static
bool check_numbers_part2(const size_t count, const size_t items SCR(count))
{
	assert(count > 2);
	static constexpr size_t limit = 3+1;
	struct {
		bool skipped;
		size_t index;
	} is = {0};

	// All increasing within limits
	if (items[0] < items[1]) {
		// putss("Increase");
		for (size_t i = 1; i != count; ++i) {
			if (!(items[i - 1] < items[i] && items[i - 1] + (limit > items[i]))) {
				is.skipped = true;
				is.index = i - 1;
				break;
			}
		}
		if (!is.skipped) { return true; }
	}
	// All decreasing within limits
	else if (items[0] > items[1]) {
		// putss("Decrease");
		for (size_t i = 1; i != count; ++i) {
			if (!(items[i - 1] > items[i] && items[i - 1] < (items[i] + limit))) {
				is.skipped = true;
				is.index = i - 1;
				break;
			}
		}
		if (!is.skipped) { return true; }
	}
	else /* Equals */ { is.skipped = true; }

	// Handle skipped here?
	const size_t *a = &items[0];
	const size_t *b = &items[1];

	if (a == &items[is.index]) { ++a; }
	if (b == &items[is.index] || a == b) { ++b; }
	if (*a < *b) {
		for (; b != &items[count]; ++a, ++b) {
			if (a == &items[is.index]) { ++a; }
			if (b == &items[is.index] || a == b) { ++b; }
			if (!(*a < *b && (*a + limit) > *b)) { return false; }
		}
		return true;
	}
	else if (*a > *b) {
		for (; b != &items[count]; ++a, ++b) {
			if (a == &items[is.index]) { ++a; }
			if (b == &items[is.index] || a == b) { ++b; }
			if (!(*a > *b && *a < (*b + limit))) { return false; }
		}
		return true;
	}

	return false;
}

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv SCR(argc)) {
	putss("Advent of Code: Day 2");

	if (argc < 2) {
		eputs("Please provide input file...");
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
	static constexpr size_t item_count = 100;
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

		// size_t skip_index = 0;
		safe_count += check_numbers_part2(count, items);
	}

	print("%zu\n", safe_count);

	deallocate(&mallocator, data);
	return 0;
}
