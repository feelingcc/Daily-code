all: server client
server: tcp_server.cc
	g++ -o $@ $^ -std=c++11 -g
client: tcp_client.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean: 
	rm -rf server client