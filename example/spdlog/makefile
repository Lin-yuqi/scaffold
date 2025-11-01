CFLAGES=-I../../source/ -std=c++17
log_test:log_test.cc ../../source/linlog.cc
	g++ $(CFLAGES) -o $@ $^ -lfmt -lspdlog -lgflags

.PHONY:clean
clean:
	rm -f log_test
