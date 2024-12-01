
#include <helpers/utility.h>
#include <helpers/file.h>
#define _need_mallocator
#include <helpers/allocator.h>

#include <assert.h>
#include <stdio.h>

int compare(const void * restrict a, const void * restrict b)
{
	const size_t *za = a;
	const size_t *zb = b;
	if (*za < *zb) { return -1; }
	if (*za > *zb) { return 1; }
	return 0;
}

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv SCR(argc)) {
	putss("Advent of Code: Day 1");

	if (argc < 2) {
		errputs("Please provide input file...");
		return 1;
	}

	const char *filename = argv[1];
	size_t data_size = 0;
	char *const data = file_dump_allocator(filename, &data_size, &mallocator);
	// Data is in format NUMBER spaces NUMBER newline
	putss(data);
	char *data_iter = data;

	// Collect each column into it's own collection.
	size_t *column_l = allocate(&mallocator, sizeof (size_t) * (data_size / 2));
	size_t *column_r = allocate(&mallocator, sizeof (size_t) * (data_size / 2));
	size_t idx = 0;
	while (data_iter && *data_iter != '\0' && idx < data_size / 2) {
		column_l[idx] = strtoul(data_iter, &data_iter, 10);
		if (column_l[idx] == 0) { break; }
		column_r[idx] = strtoul(data_iter, &data_iter, 10);
		// printfs("%zu %zu\n", column_l[idx], column_r[idx]);
		++idx;
	}

	// Sort each column from smallest to largest.
	// Abusing qsort
	qsort(column_l, idx, sizeof (size_t), compare);
	qsort(column_r, idx, sizeof (size_t), compare);

	// Iterate over both columns, measure distance = abs(a - b)
	// Sum up distances for total distance.
	size_t sum = 0;
	for (size_t i = 0; i != idx; ++i) {
		sum += (column_l[i] > column_r[i]) ? (column_l[i] - column_r[i]) : (column_r[i] - column_l[i]);
	}

	printfs("Distance = %zu\n", sum);

	// Part 2: Similarity Score
	static size_t cache_size = 1024;
	struct {
		bool entry;
		unsigned char data[sizeof (size_t) - 1];
	} *cache = allocate(&mallocator, sizeof (*cache) * cache_size);

	size_t similarity_score = 0;
	for (size_t i = 0; i != idx; ++i) {
		size_t number = column_l[i];
		if (number > cache_size) {
			cache_size *= 2;
			cache = reallocate(&mallocator, cache, sizeof (*cache) * cache_size);
		}
		size_t count = 0;
		if (cache[number].entry == true) {
			(memcpy(&count, cache[number].data, sizeof (size_t) - 1));
		}
		else {
			for (size_t j = 0; j != idx; ++j) {
				if (column_r[j] == number) { ++count; }
			}
			memcpy(&cache[number].data, &count, sizeof (count) - 1);
			cache[number].entry = true;
		}
		similarity_score += number * count;
	}

	printfs("Similarity score: %zu\n", similarity_score);

	deallocate(&mallocator, column_l);
	deallocate(&mallocator, column_r);
	deallocate(&mallocator, data);
	return 0;
}
