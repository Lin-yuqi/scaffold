all: string
string:string.cc
	g++ -o $@ $^ -std=c++17 -lhiredis -lredis++ -lpthread
.PHONY:clean
clean:
	rm -f string