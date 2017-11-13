#include "signalhandler.h"
#include "modulepool.h"
#include "processpool.h"
#include <signal.h>
#include <cstring>
#include <cstdio>
static void singalhandler(int signo,siginfo_t*siginfo,void*ucontext){
    switch (signo){
    case SIGINT:
        processpool::get()->reconfigureflag=1;
        break;
    case SIGCHLD:
        processpool::get()->waitflag=1;
        break;
    default:
        break;
    }
}
int initsignal(){
    struct sigaction sa;
    memset(&sa,0,sizeof(struct sigaction));
    sa.sa_sigaction=singalhandler;
    sa.sa_flags=SA_SIGINFO|SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT,&sa,NULL);
    sigaction(SIGCHLD,&sa,NULL);
    return 0;
}