.PHONY:all
all: udpserver udpclient

udpserver:UdpServer.cc
	g++ -o $@ $^ -std=c++17	-lpthread
udpclient:UdpClient.cc
	g++ -o $@ $^ -std=c++17 -lpthread

.PHONY:clean
clean:
	rm -f udpclient udpserver