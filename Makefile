CXXFLAGS+=-std=c++11
LDFLAGS=-shared -fPIC
all:server a_time.so b_version.so c_disk.so d_process.so
obj=server.o processpool.o
server:$(obj)
	g++ -rdynamic -o server $(obj) -ldl
processpool.o:processpool.h process.h
a_time.so:
	g++ time.cpp $(LDFLAGS) $(CXXFLAGS) -o a_time.so
b_version.so:
	g++ version.cpp $(LDFLAGS) $(CXXFLAGS) -o b_version.so
c_disk.so:
	g++ disk.cpp $(LDFLAGS) $(CXXFLAGS) -o c_disk.so
d_process.so:
	g++ process.cpp $(LDFLAGS) $(CXXFLAGS) -o d_process.so
clean:
	rm $(obj) *.so
