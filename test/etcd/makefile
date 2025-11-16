all:discovery register
discovery:discovery.cc ../../source/util.cc ../../source/log.cc ../../source/etcd.cc
	g++ -std=c++17 -o $@ $^ -lfmt -pthread -ljsoncpp -lcpprest -letcd-cpp-api
register:register.cc ../../source/util.cc ../../source/log.cc ../../source/etcd.cc
	g++ -g -std=c++17 -o $@ $^ -lfmt -pthread -ljsoncpp -lcpprest -letcd-cpp-api

clean:
	rm -f discovery register
