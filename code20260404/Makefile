.PHONY:all
all: rpc_client rpc_server async_rpc_server async_rpc_client

rpc_server: rpc_server.cc cal.pb.cc 
	g++ -std=c++17 -o $@ $^ -lbrpc -lleveldb -lprotobuf -lpthread -ldl -lssl -lcrypto -lgflags

rpc_client: rpc_client.cc cal.pb.cc  
	g++ -std=c++17 -o $@ $^ -lbrpc -lleveldb -lprotobuf -lpthread -ldl -lssl -lcrypto -lgflags

async_rpc_server: async_rpc_server.cc cal.pb.cc 
	g++ -std=c++17 -o $@ $^ -lbrpc -lleveldb -lprotobuf -lpthread -ldl -lssl -lcrypto -lgflags

async_rpc_client: async_rpc_client.cc cal.pb.cc  
	g++ -std=c++17 -o $@ $^ -lbrpc -lleveldb -lprotobuf -lpthread -ldl -lssl -lcrypto -lgflags

%.pb.cc: %.proto
	protoc --cpp_out=. $^

.PHONY:clean
clean:
	rm -f rpc_server rpc_client async_rpc_client async_rpc_server