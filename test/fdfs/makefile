fdfs_test:fdfs_test.cc ../../source/fdfs.cc ../../source/util.cc ../../source/log.cc
	g++ -g -o $@ $^ -std=c++17 -lfmt -lspdlog -ljsoncpp -lfastcommon -lfdfsclient -pthread
clean:
	rm -f fdfs_test