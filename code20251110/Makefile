.PHONY:clean
clean: tcpserver

tcpserver: TcpServer.cc
	g++ -o $@ $^ -std=c++17

.PHONY:clean
clean:
	rm -f tcpserver