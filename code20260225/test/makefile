.PHONY:all
all: tcp_client tcp_server

tcp_client: tcp_client.cc
	g++ -std=c++17 -o $@ $^

tcp_server: tcp_server.cc
	g++ -std=c++17 -o $@ $^ -g

.PHONY:clean
clean:
	rm -f tcp_client tcp_server
