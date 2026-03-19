.PHONY:all
all: client4

client4:client4.cpp
	g++ -std=c++17 $^ -o $@ -g

client3:client3.cpp
	g++ -std=c++17 $^ -o $@

client2:client2.cpp
	g++ -std=c++17 $^ -o $@

client1:client1.cpp
	g++ -std=c++17 $^ -o $@

server: server.cc
	g++ -std=c++17 -o $@ $^

tcp_client: tcp_client.cc
	g++ -std=c++17 -o $@ $^

tcp_server: tcp_server.cc
	g++ -std=c++17 -o $@ $^ -g

.PHONY:clean
clean:
	rm -f tcp_client tcp_server server
