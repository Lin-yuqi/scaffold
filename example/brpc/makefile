all: sync_server sync_client async_client async_server http_client
sync_server: sync_server.cc cal.pb.cc
	g++ -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags
sync_client: sync_client.cc cal.pb.cc
	g++ -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags
async_client: async_client.cc cal.pb.cc
	g++ -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags
async_server: async_server.cc cal.pb.cc
	g++ -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags
http_client: http_client.cc cal.pb.cc
	g++ -std=c++17 -o $@ $^ -lbrpc -lpthread -lleveldb -lprotobuf -ldl -pthread -lssl -lcrypto -lgflags
.PHONY: clean
clean:
	rm -f sync_server sync_client async_server async_client