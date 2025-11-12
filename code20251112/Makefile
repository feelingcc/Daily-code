.PHONY:clean
clean: tcpserver

tcpserver: TcpServer.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp

.PHONY:clean
clean:
	rm -f tcpserver