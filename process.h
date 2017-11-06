#ifndef PROCESS_H
#define PROCESS_H

#include<unistd.h>
#define SPAWN_ZERO 0
#define SPAWN_ONE 1
#define SPAWN_TWO 2
class process{
public:
	process():mypid(0),spawn(SPAWN_ZERO){};
	pid_t mypid;
	int spawn;
	//int pipefd[2];
};
#endif
