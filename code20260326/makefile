.PHONY:all
all: test

test: test.cc util.cc
	g++ -std=c++17 -o $@ $^ -ljsoncpp

main: json.cc
	g++ -std=c++17 -o $@ $^ -ljsoncpp