#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H

#include"process.h"
#define MAX_PROCESSES 128
class processpool{
private:
	processpool(int listenfd,int processnum);
	int processnum;
	int listenfd;
	process*subprocess;
	void runparent();
	void runchild();
	void spawnprocess(int i,int spawn);
	static processpool*pool;
public:
	~processpool();
	void run();
	static processpool*get(int listenfd,int processnum);
};
#endif
