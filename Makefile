LDFLAGS=-shared -fPIC
all:server a_time.so b_version.so c_disk.so d_process.so
obj=module.o response.o modulepool.o signalhandler.o
response.o:response.h
module.o:module.h
modulepool.o:modulepool.h
signalhandler.o:signalhandler.h
server:server.o processpool.o $(obj)
	g++ -rdynamic -o server $(obj) server.o processpool.o -ldl
processpool.o:processpool.h process.h
a_time.so:
	g++ time.cpp $(LDFLAGS) -o a_time.so
b_version.so:
	g++ version.cpp $(LDFLAGS) -o b_version.so
c_disk.so:
	g++ disk.cpp $(LDFLAGS) -o c_disk.so
d_process.so:
	g++ process.cpp $(LDFLAGS) -o d_process.so
clean:
	rm *.o *.so
