#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H

#include"process.h"
#define MAX_PROCESSES 128
class processpool{
private:
	processpool(int listenfd,int processnum);
	int processnum;
	int processlast;
	int listenfd;
	process*subprocess;
	void runparent();
	void runchild();
	static processpool*pool;
public:
	void spawnprocess(int spawn);
	~processpool();
	void run();
	static processpool*get(int listenfd,int processnum);
	static processpool*get();
	void reconfigure();
};
#endif
