#ifndef PROCESS_H
#define PROCESS_H
#include<unistd.h>
#include<sys/types.h>
class process{
public:
	process():mypid(-1){};
	pid_t mypid;
	int pipefd[2];
};
#endif
