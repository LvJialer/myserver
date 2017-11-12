#include "signalhandler.h"
#include "modulepool.h"
#include "processpool.h"
#include <signal.h>
#include <cstring>
#include <cstdio>
extern int processid;
static void singalhandler(int signo,siginfo_t*siginfo,void*ucontext){
    if(processid==-1){
        printf("b\n");
        fflush(stdout);
        modulepool::get()->reload();
        processpool::get()->spawnprocess(SPAWN_TWO);
        processpool::get()->reconfigure();
        processpool::get()->run();
    }
}
int initsignal(){
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_sigaction=singalhandler;
    sa.sa_flags=SA_SIGINFO|SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT,&sa,NULL);
    return 0;
}