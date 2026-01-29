json_test: json_test.cc ../../source/util.cc ../../source/linlog.cc
	g++ -o json_test $^ -ljsoncpp -std=c++17 -lfmt -lspdlog -lpthread

clean:
	rm -f json_test 