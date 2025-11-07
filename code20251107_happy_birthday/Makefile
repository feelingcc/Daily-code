.PHONY:all
all: tcpserver tcpclient

tcpserver: TcpServer.cc
	g++ -o $@ $^ -std=c++17
tcpclient: TcpClient.cc
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f tcpclient tcpserver
