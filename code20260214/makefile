test-redis-plus-plus: redis-plus-plus.cc
	g++ -std=c++17 -o $@ $^ -lredis++ -lhiredis -pthread

.PHONY:clean
clean:
	rm -rf test-redis-plus-plus