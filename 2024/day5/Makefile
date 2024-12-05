.PHONY: all test clean debug release

all: debug release

CSTD := -std=c23

debug: CFLAGS += $(CSTD) -Wno-old-style-declaration -Wall -Wextra -Wshadow -Wconversion -pedantic-errors -fanalyzer -fsanitize=address,bounds,undefined -DDEBUG -g3
debug: main.c
	$(CC) -o $@ $^ $(CFLAGS) $(CLIBS) $(CINCLUDE)

release: CFLAGS += $(CSTD) -pedantic-errors -O3 -DNDEBUG
release: main.c
	$(CC) -o $@ $^ $(CFLAGS) $(CLIBS) $(CINCLUDE)
