hls_test:test.cc ../../source/hls.cc ../../source/log.cc ../../source/util.cc
	g++ -std=c++17 -o $@ $^ -lavformat -lavcodec -lavutil  -lfmt -lspdlog -ljsoncpp 

.PHONY:clean
clean:
	rm -f hls_test