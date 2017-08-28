obj=server.o processpool.o
server:$(obj)
	g++ -rdynamic -o server $(obj) -ldl
processpool.o:processpool.h process.h
clean:
	rm $(obj)
