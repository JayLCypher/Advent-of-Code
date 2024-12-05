
#include <helpers/utility.h>
#include <helpers/file.h>
#include <helpers/mallocator.h>

#include <stdio.h>
#include <string.h>


typedef struct rule rule;
struct rule {
	size_t before;
	size_t after;
};

static
bool check_ordering(const size_t count, const size_t items SCR(1), const size_t rule_count, const rule rules SCR(rule_count))
{
	for (size_t i = 0; i != rule_count; ++i) {
		const rule *r = &rules[i];
		for (size_t j = 0; j != count; ++j) {
			if (items[j] == r->before) {
				for (size_t k = 0; k != j; ++k) {
					if (items[k] == r->after) {
						println("%zu(%zu) violates rule %zu: %zu|%zu", k, items[k], i, r->before, r->after);
						return false;
					}
				}
			}
			if (items[j] == r->after) {
				for (size_t k = count; k != j; --k) {
					if (items[k] == r->before) {
						println("%zu(%zu) violates rule %zu: %zu|%zu", k, items[k], i, r->before, r->after);
						return false;
					}
				}
			}
		}
	}
	return true;
}

static
void fix_ordering(const size_t item_count, size_t items SCR(item_count), const size_t rule_count, const rule rules SCR(rule_count))
{
	for (size_t i = 0; i != item_count; ++i) {
		size_t *item = &items[i];
		for (size_t j = 0; j != rule_count; ++j) {
			const rule *rp = &rules[j];
			if (*item == rp->before) {
				for (size_t k = item_count; k != i; --k) {
					if (items[k] == rp->after) {
						swap(items[k], *item);
					}
				}
				continue;
			}
			if (*item == rp->after) {
				for (size_t k = 0; k != i; ++k) {
					if (items[k] == rp->before) {
						swap(items[k], *item);
					}
				}
				continue;
			}
		}
	}
}

static inline
size_t get_middle_page(const size_t count, const size_t items SCR(count))
{ return items[count / 2]; }

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char *argv SCR(argc)) {
	putss("Advent of Code: Day 5!");

	if (argc < 2) {
		eputs("Please provide input file...");
		return 1;
	}

	const char *filename = argv[1];
	size_t data_size = 0;
	char *const data = file_slurp_allocator(filename, &data_size, &mallocator);
	putss(data);

	// Gather rules.
	rule rules[2048] = {0};
	size_t rule_count = 0;
	char *data_iter = data;
	for (;;) {
		rules[rule_count].before = strtoul(data_iter, &data_iter, 10);
		assert(*data_iter == '|');
		++data_iter;
		rules[rule_count].after = strtoul(data_iter, &data_iter, 10);
		assert(*data_iter == '\n');
		++data_iter;
		if (*data_iter == '\n') { break; }
		++rule_count;
		assert(rule_count < (sizeof (rules) / sizeof (*rules)));
	}
	++data_iter; // Skip \n

	size_t accumulator = 0;
	size_t eccumulator = 0;
	size_t items[1024] = {0};
	size_t item_count = 0;
	while (data_iter && *data_iter != '\0') {
		for (;;) {
			items[item_count++] = strtoul(data_iter, &data_iter, 10);
			if (*data_iter == '\n') { break; }
			assert(*data_iter == ',');
			++data_iter;
		}
		if (check_ordering(item_count, items, rule_count, rules)) {
			const size_t middle_page = get_middle_page(item_count, items); 
			println("  adding %zu", middle_page);
			accumulator += middle_page;
		}
		else { 
			fix_ordering(item_count, items, rule_count, rules);
			const size_t middle_page = get_middle_page(item_count, items); 
			println("  adding %zu", middle_page);
			eccumulator += middle_page;
		}
		++data_iter;
		check(memset(items, 0, sizeof (*items) * item_count) != nullptr);
		item_count = 0;
	}

	println("Sum of middle pages: %zu", accumulator);
	const bool is_example = (strncmp(filename, "example.txt", sizeof ("example.txt")) == 0); 
	if (is_example && accumulator != 143) {
		println("ERROR: Expected %zu, got %zu instead.", 143LU, accumulator);
	}
	println("Sum of fixed middle pages: %zu", eccumulator);
	if (is_example && eccumulator != 123) {
		println("ERROR: Expected %zu, got %zu instead.", 123LU, eccumulator);
	}

	deallocate(&mallocator, data);
	return 0;
}
