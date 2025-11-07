all: svr clt
svr: rpc_server.cc cal.pb.cc ../../source/linrpc.cc ../../source/linlog.cc
	g++ -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags -lfmt -lspdlog
clt: rpc_client.cc cal.pb.cc ../../source/linrpc.cc ../../source/linlog.cc
	g++ -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags -lfmt -lspdlog

%.proto.cc: %.proto
	protoc --cpp_out=./ $^

.PHONY:clean
clean:
	rm -f svr clt 