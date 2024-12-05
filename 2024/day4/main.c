
#include <helpers/utility.h>
#include <helpers/file.h>
#include <helpers/mallocator.h>

#include <stdio.h>
#include <string.h>

static constexpr size_t part = 2;
static constexpr size_t expected[] = {
	0,
	18,
	9
};

typedef struct rect rect;
struct rect {
	size_t x;
	size_t y;
	size_t width;
	size_t height;
};

static
size_t is_xmas1(const char *restrict data, rect r);
static
size_t is_xmas2(const char *restrict data, rect r);

static inline
size_t lines_in_cstr(const char *restrict str)
{
	size_t count = 0;
	while ((str = strchr(str + 1, '\n'))) { ++count; }
	return count;
}

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv SCR(argc)) {
	putss("Advent of Code: Day 4!");

	if (argc < 2) {
		eputs("Please provide input file...");
		return 1;
	}

	const char *filename = argv[1];
	size_t data_size = 0;
	char *const data = file_slurp_allocator(filename, &data_size, &mallocator);
	putss(data);

	static rect r = {0};
	r.width = (size_t)(strchr(data, '\n') - data);
	println("Width: %zu", r.width);
	r.height = lines_in_cstr(data);
	println("Height: %zu", r.height);

	static size_t match_count = 0;
	while (true) {
		if (part == 1) { match_count += is_xmas1(data, r); }
		else if (part == 2) { match_count += is_xmas2(data, r); }
		++r.x;
		if (r.x == r.width) {
			if (r.y == r.height - 1) { break; }
			r.x = 0;
			++r.y;
		}
	}
	println("XMAS Matches: %zu", match_count);
	if (strncmp(filename, "example.txt", sizeof ("example.txt")) == 0 && match_count != expected[part]) {
		println("[ERROR] Expected %zu, but got %zu.", expected[part], match_count);
	}

	deallocate(&mallocator, data);
	return 0;
}

static
size_t is_xmas1(const char *restrict data, const rect r)
{
	static const char *const restrict needle = "XMAS";
	static constexpr int bounds = 4;
	static char window[bounds] = {0};
	const size_t bound_width = r.width+1;
	size_t count = 0;
	// Forward/Right
	if (r.x + bounds < bound_width) {
		println("  Right(%zu, %zu): %.*s", r.x, r.y, bounds, &data[r.x + (r.y * bound_width)]);
		if (strncmp(&data[r.x + (r.y * bound_width)], needle, bounds) == 0) { ++count; }
		// RightDown
		if (r.y + bounds-1 < r.height) {
			window[0] = data[(r.x + 0) + ((r.y + 0) * bound_width)];
			window[1] = data[(r.x + 1) + ((r.y + 1) * bound_width)];
			window[2] = data[(r.x + 2) + ((r.y + 2) * bound_width)];
			window[3] = data[(r.x + 3) + ((r.y + 3) * bound_width)];
			println("  RightDown(%zu, %zu): %.*s", r.x, r.y, bounds, window);
			if (strncmp(window, needle, bounds) == 0) { ++count; }
		}
		// RightUp
		if (r.y > 2) {
			window[0] = data[(r.x + 0) + ((r.y - 0) * bound_width)];
			window[1] = data[(r.x + 1) + ((r.y - 1) * bound_width)];
			window[2] = data[(r.x + 2) + ((r.y - 2) * bound_width)];
			window[3] = data[(r.x + 3) + ((r.y - 3) * bound_width)];
			println("  RightUp(%zu, %zu): %.*s", r.x, r.y, bounds, window);
			if (strncmp(window, needle, bounds) == 0) { ++count; }
		}
	}
	// Backward/Left
	if (r.x > 2) {
		window[0] = data[(r.x - 0) + (r.y * bound_width)];
		window[1] = data[(r.x - 1) + (r.y * bound_width)];
		window[2] = data[(r.x - 2) + (r.y * bound_width)];
		window[3] = data[(r.x - 3) + (r.y * bound_width)];
		println("  Left(%zu, %zu): %.*s", r.x, r.y, bounds, window);
		if (strncmp(window, needle, bounds) == 0) { ++count; }
		// LeftDown
		if (r.y + bounds-1 < r.height) {
			window[0] = data[(r.x - 0) + ((r.y + 0) * bound_width)];
			window[1] = data[(r.x - 1) + ((r.y + 1) * bound_width)];
			window[2] = data[(r.x - 2) + ((r.y + 2) * bound_width)];
			window[3] = data[(r.x - 3) + ((r.y + 3) * bound_width)];
			println("  LeftDown(%zu, %zu): %.*s", r.x, r.y, bounds, window);
			if (strncmp(window, needle, bounds) == 0) { ++count; }
		}
		// LeftUp
		if (r.y > 2) {
			window[0] = data[(r.x - 0) + ((r.y - 0) * bound_width)];
			window[1] = data[(r.x - 1) + ((r.y - 1) * bound_width)];
			window[2] = data[(r.x - 2) + ((r.y - 2) * bound_width)];
			window[3] = data[(r.x - 3) + ((r.y - 3) * bound_width)];
			println("  LeftUp(%zu, %zu): %.*s", r.x, r.y, bounds, window);
			if (strncmp(window, needle, bounds) == 0) { ++count; }
		}
	}
	// Down
	if (r.y + bounds-1 < r.height) {
		window[0] = data[r.x + ((r.y + 0) * bound_width)];
		window[1] = data[r.x + ((r.y + 1) * bound_width)];
		window[2] = data[r.x + ((r.y + 2) * bound_width)];
		window[3] = data[r.x + ((r.y + 3) * bound_width)];
		println("  Down(%zu, %zu): %.*s", r.x, r.y, bounds, window);
		if (strncmp(window, needle, bounds) == 0) { ++count; }
	}
	// Up
	if (r.y > 2) {
		window[0] = data[r.x + ((r.y - 0) * bound_width)];
		window[1] = data[r.x + ((r.y - 1) * bound_width)];
		window[2] = data[r.x + ((r.y - 2) * bound_width)];
		window[3] = data[r.x + ((r.y - 3) * bound_width)];
		println("  Up(%zu, %zu): %.*s", r.x, r.y, bounds, window);
		if (strncmp(window, needle, bounds) == 0) { ++count; }
	}
	return count;
}

static inline
bool check_xmas_char(const char a, const char b)
{
	return ((a == 'M' && b == 'S') || (a == 'S' && b == 'M'));
}

static
size_t is_xmas2(const char *restrict data, rect r)
{
	if (r.x + 3 > r.width) { return 0; }
	if (r.y + 3 > r.height) { return 0; }

	static char window[3][3] = {0};
	const size_t bound_width = r.width + 1; 
	window[0][0] = data[(r.x + 0) + ((r.y + 0) * bound_width)];
	window[0][1] = data[(r.x + 1) + ((r.y + 0) * bound_width)];
	window[0][2] = data[(r.x + 2) + ((r.y + 0) * bound_width)];

	window[1][0] = data[(r.x + 0) + ((r.y + 1) * bound_width)];
	window[1][1] = data[(r.x + 1) + ((r.y + 1) * bound_width)];
	window[1][2] = data[(r.x + 2) + ((r.y + 1) * bound_width)];

	window[2][0] = data[(r.x + 0) + ((r.y + 2) * bound_width)];
	window[2][1] = data[(r.x + 1) + ((r.y + 2) * bound_width)];
	window[2][2] = data[(r.x + 2) + ((r.y + 2) * bound_width)];

	putss("Window:");
	for (size_t i = 0; i != sizeof (window) / 3; ++i) {
		println("%.*s", (int)(sizeof (window)/3), window[i]);
	}

	if (window[1][1] != 'A') { return 0; }
	if (check_xmas_char(window[0][0], window[2][2]) &&
		check_xmas_char(window[2][0], window[0][2])) {
		return 1;
	}

	return 0;
}
