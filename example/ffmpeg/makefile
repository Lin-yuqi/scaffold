all:hls
hls:hls.cc
	g++ -std=c++17 -o $@ $^ -lavformat -lavcodec -lavutil 
.PHONY:clean
clean:
	rm -f hls