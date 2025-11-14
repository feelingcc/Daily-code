.PHONY:all
all: tcpserver tcpclient

tcpserver: TcpServer.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp
tcpclient: TcpClient.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp

.PHONY:clean
clean:
	rm -f tcpserver tcpclient