all: svr clt
svr: rpc_server.cc cal.pb.cc ../../source/rpc.cc ../../source/log.cc ../../source/etcd.cc ../../source/util.cc
	g++ -g -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags -lfmt -lspdlog -ljsoncpp -lcpprest -letcd-cpp-api
clt: rpc_client.cc cal.pb.cc ../../source/rpc.cc ../../source/log.cc ../../source/etcd.cc ../../source/util.cc
	g++ -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags -lfmt -lspdlog -ljsoncpp -lcpprest -letcd-cpp-api

%.proto.cc: %.proto
	protoc --cpp_out=./ $^

.PHONY:clean
clean:
	rm -f svr clt 