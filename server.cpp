#include"processpool.h"
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<cstring>
#include<cstdlib>
#include"modulepool.h"
#include"signalhandler.h"
#define PROCESS_NUM 4
using namespace std;
int processid;
int main(int argc,char*argv[]){
	cout<<"Welcome to myserver!"<<endl;
	initsignal();
	modulepool*modulepools=modulepool::get();
	processid=-1;
	int listenfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenfd<0){cout<<"socket error"<<endl;exit(1);}
	struct sockaddr_in address;
	const int PORT=1888;
	memset(&address,0,sizeof(address));
	address.sin_family=AF_INET;
	address.sin_port=htons(PORT);
	address.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(listenfd,reinterpret_cast<struct sockaddr*>(&address),sizeof(address))<0){cout<<"bind error"<<endl;exit(1);}
	if(listen(listenfd,5)<0){cout<<"listen error"<<endl;exit(1);}
	processpool*pool=processpool::get(listenfd,PROCESS_NUM);
	if(pool){
		pool->run();
	}
	close(listenfd);
	return 0;
}
