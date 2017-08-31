#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H
#include"process.h"
class processpool{
private:
	processpool(int listenfd,int processnum=4);
	int processnum;
	int listenfd;
	process*subprocess;
	int idx;
	void runparent();
	void runchild();
	static processpool*pool;
public:
	~processpool();
	void run();
	static processpool*get(int listenfd);
};
#endif
