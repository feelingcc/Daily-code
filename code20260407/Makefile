all: etcd_client etcd_server
etcd_client:etcd_client.cc
	g++ -std=c++17 $^ -o $@ -lcpprest -letcd-cpp-api
etcd_server:etcd_server.cc
	g++ -std=c++17 $^ -o $@ -lcpprest -letcd-cpp-api

clean:
	rm -rf etcd_client etcd_server