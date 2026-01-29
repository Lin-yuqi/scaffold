all: delayed_publish delayed_subscribe
simple_publish:simple_publish.cc ../../source/mq.cc ../../source/log.cc
	g++ -std=c++17 -o $@ $^ -lfmt -lspdlog -lamqpcpp -lpthread -lev
simple_subscribe:simple_subscribe.cc ../../source/mq.cc ../../source/log.cc
	g++ -std=c++17 -o $@ $^ -lfmt -lspdlog -lamqpcpp -lpthread -lev
delayed_publish:delayed_publish.cc ../../source/mq.cc ../../source/log.cc
	g++ -std=c++17 -o $@ $^ -lfmt -lspdlog -lamqpcpp -lpthread -lev
delayed_subscribe:delayed_subscribe.cc ../../source/mq.cc ../../source/log.cc
	g++ -std=c++17 -o $@ $^ -lfmt -lspdlog -lamqpcpp -lpthread -lev

clean:
	rm -f simple_publish simple_subscribe delayed_publish delayed_subscribe