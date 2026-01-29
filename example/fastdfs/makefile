all:upload remove download

upload:upload.cc
	g++ -o $@ $^ -std=c++11 -lfdfsclient -lfastcommon -pthread
remove:remove.cc
	g++ -o $@ $^ -std=c++11 -lfdfsclient -lfastcommon -pthread
download:download.cc
	g++ -o $@ $^ -std=c++11 -lfdfsclient -lfastcommon -pthread

.PHONY:clean
clean:
	rm -f upload remove download