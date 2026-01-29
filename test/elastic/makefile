all:base_test es_test
es_test:es_test.cc ../../source/elastic.cc ../../source/util.cc ../../source/log.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp -lfmt -pthread -lspdlog -lelasticlient
base_test:base_test.cc ../../source/elastic.cc ../../source/util.cc ../../source/log.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp -lfmt -pthread -lspdlog -lelasticlient 
.PHONY:clean
clean:
	rm -f base_test es_test