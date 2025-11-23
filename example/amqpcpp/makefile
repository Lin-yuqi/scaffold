all: simple_publish simple_subscribe dlx_publish dlx_subscribe
simple_publish:simple_publish.cc
	g++ -o $@ $^ -lamqpcpp -lpthread -lev
simple_subscribe:simple_subscribe.cc
	g++ -o $@ $^ -lamqpcpp -lpthread -lev
dlx_publish:dlx_publish.cc
	g++ -o $@ $^ -lamqpcpp -lpthread -lev
dlx_subscribe:dlx_subscribe.cc
	g++ -o $@ $^ -lamqpcpp -lpthread -lev
clean:
	rm -f simple_publish simple_subscribe dlx_publish dlx_subscribe