.PHONY:all
all: udpserver udpclient

udpserver:UdpServer.cc
	g++ -o $@ $^ -std=c++17
udpclient:UdpClient.cc
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f udpclient udpserver