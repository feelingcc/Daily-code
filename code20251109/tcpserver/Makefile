.PHONY:all
all: tcpserver tcpclient

tcpserver: TcpServer.cc
	g++ -o $@ $^ -std=c++17 -lpthread
tcpclient: TcpClient.cc
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f tcpclient tcpserver
