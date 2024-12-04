
#include <helpers/utility.h>
#include <helpers/file.h>
#include <helpers/mallocator.h>

#include <stdio.h>
#include <string.h>

static size_t do_match = 0; // 35
static size_t dont_match = 0; // 43
static size_t mul_match = 0; // 738

static inline
bool check_num(const char c)
{ return (c > '0'-1 && c < '9'+1); }

size_t check_mul(const char **data)
{
	if (strncmp(*data, "mul", 3) != 0) { return 0; }
	*data += 3;
	if (**data != '(') { return 0; }
	++*data;
	const char *lhs_ptr = *data;
	for (size_t max = 3; **data != ',' && max != 0; --max) {
		if (check_num(**data)) { ++*data; }
	}
	if (**data != ',') { return 0; }
	++*data;
	const char *rhs_ptr = *data;
	for (size_t max = 3; **data != ')' && max != 0; --max) {
		if (check_num(**data)) { ++*data; }
	}
	if (**data != ')') { return 0; }
	++*data;
	const auto lhs = strtoul(lhs_ptr, nullptr, 10);
	const auto rhs = strtoul(rhs_ptr, nullptr, 10);
	++mul_match;
	return lhs * rhs;
}

int check_dos(const char **data) {
	int ret = 0;
	if (strncmp(*data, "do", 2) == 0) {
		*data += 2;
		ret = 1;
		++do_match;
	}
	if (strncmp(*data, "n't", 3) == 0) {
		*data += 3;
		ret = -1;
		--do_match;
		++dont_match;
	}
	if (**data != '(') { return 0; }
	++*data;
	if (**data != ')') { return 0; }
	++*data;
	return ret;
}

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv SCR(argc)) {
	putss("Advent of Code: Day 3!");

	if (argc < 2) {
		errputs("Please provide input file...");
		return 1;
	}

	const char *filename = argv[1];
	size_t data_size = 0;
	char *const data = file_slurp_allocator(filename, &data_size, &mallocator);
	// Data is in format NUMBER spaces NUMBER newline
	// putss(data);
	putss("Iterating:");

	static int ignore = 0;
	size_t sum = 0;
	const char *data_iter = data;
	for (size_t i = 0; data_iter && i != data_size; ++i) {
		const char *data_iter2 = strpbrk(data_iter, "dm");
		if (!data_iter2) { break; }
		long dist = (data_iter2 - data_iter);
		if (*data_iter2 == 'd') {
			ignore = check_dos(&data_iter2);
			if (ignore != 0) { printf("  ignore: %d\n", ignore); }
		}
		auto result = check_mul(&data_iter2);
		if (result && ignore >= 0) {
			print("  %.*s: %zu\n", (int)(data_iter2 - data_iter - dist), (data_iter + dist), result);
			sum += result;
		}

		data_iter = data_iter2 + (result == 0);
	}

	putss("Matches:");
	print("  Do:    %zu\n", do_match);
	print("  Don't: %zu\n", dont_match);
	print("  Mul:   %zu\n", mul_match);

	print("Sum: %zu\n", sum); // 738 counts for part 1.

	deallocate(&mallocator, data);
	return 0;
}
