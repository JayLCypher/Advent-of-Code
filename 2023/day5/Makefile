.PHONY: all test clean debug release

CXXVERSION = -std=c++26
CXXFLAGS = -Wall -Wextra -Wshadow -Wconversion
SRC = day5.cpp

all: debug release

debug: CXXFLAGS += -fanalyzer -fsanitize=address,bounds,undefined -DDEBUG -g3
debug: day5.cpp
	$(CXX) -o $@ $^ $(CXXVERSION) $(CXXFLAGS) $(LFLAGS)

release: CXXFLAGS += -O3 -DNDEBUG
release: day5.cpp
	$(CXX) -o $@ $^ $(CXXVERSION) $(CXXFLAGS) $(LFLAGS)

clean:
	rm -rf $(OUT)
