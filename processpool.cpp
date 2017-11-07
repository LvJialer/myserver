#include"processpool.h"
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/wait.h>
#include<iostream>
#include<dlfcn.h>
#include<cstring>
#include<cstdlib>
#include"modulepool.h"
#include"response.h"
#include<cstdio>
using namespace std;
extern int processid;
processpool::processpool(int listenfd,int processnum):listenfd(listenfd),processnum(processnum){
	subprocess=new process[MAX_PROCESSES];
	for(int i=0,j=0;j<processnum;i++){
		//if(pipe(subprocess[i].pipefd)<0){cout<<"pipe num "<<i<<" error"<<endl;}
		if(subprocess[i].spawn==SPAWN_ZERO){
			subprocess[i].spawn=SPAWN_ONE;
			subprocess[i].mypid=fork();
			if(subprocess[i].mypid<0){cout<<"fork num "<<i<<" error"<<endl;}
			if(subprocess[i].mypid>0){
				j++;
				//close(subprocess[i].pipefd[0]);
				continue;
			}
			else{
				//close(subprocess[i].pipefd[1]);
				processid=i;
				break;
			}
		}
	}
}
processpool*processpool::get(int listenfd,int processnum){
	if(pool==NULL){
		pool=new processpool(listenfd,processnum);
	}
	return pool;
}
processpool::~processpool(){
	delete[]subprocess;
	delete pool;
}
void processpool::run(){
	if(processid!=-1){runchild();return;}
	runparent();
}
void processpool::runchild(){
	for(;;){/*
		int b;int*buf=&b;
		read(subprocess[idx].pipefd[0],buf,sizeof(int));
		if(*buf==idx){*/
			struct sockaddr_in clientaddr;
			unsigned int len=sizeof(clientaddr);
			int newsock=accept(listenfd,reinterpret_cast<sockaddr*>(&clientaddr),&len);
			if(newsock<0){cout<<"accept error"<<endl;}
			else{cout<<"Accept success and child process No."<<processid<<" is working."<<endl;}
			char head[1024];
			memset(head,'\0',1024*sizeof(char));
			read(newsock,head,1024);
			char method[10];
			memset(method,'\0',10*sizeof(char));
			char url[100];
			memset(url,'\0',100*sizeof(char));
			sscanf(head,"%s %s",method,url);
			strcpy(url,url+1);
			response*res=new response();
			bool ok=false;
			modulepool*modulepools=modulepool::get();
			for(int i=0;i<modulepools->module_num;i++){
				if(strcmp(modulepools->modules[i]->name,url)==0){
					ok=true;
					modulepools->modules[i]->command(res);
				}
			}
			if(!ok){
				FILE*fd;
				if(strcmp("index.html",url)==0){
					fd=fopen("index.html","r");
				}
				else{
					fd=fopen("error.html","r");
				}
				fread(res->body,1,4096,fd);
				fclose(fd);
			}
			char sen[4096];
			sprintf(sen,"HTTP/1.1 200 OK\r\n");
			sprintf(sen,"%sServer:myserver\r\n",sen);
			sprintf(sen,"%sContent-length:%lu\r\n",sen,strlen(res->body));
			sprintf(sen,"%sContent-type:text/html\r\n\r\n",sen);
			sprintf(sen,"%s%s",sen,res->body);
			send(newsock,sen,strlen(sen),0);
			close(newsock);
		//}
	}
}
void processpool::runparent(){/*
	int epollfd=epoll_create(5);
	epoll_event event;
	event.events=EPOLLIN|EPOLLET;
	event.data.fd=listenfd;
	if(epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&event)<0){cout<<"epoll_ctl error"<<endl;exit(1);}
	epoll_event events[5];
	for(int j=0;j<10000;j++){
		int i=j%4;
		if(epoll_wait(epollfd,events,5,-1)<0){cout<<"epoll_wait error"<<endl;continue;}
		if(events[0].data.fd==listenfd){
			int b=i;
			int*buf=&b;
			write(subprocess[i].pipefd[1],buf,sizeof(int));
		}
	}
	close(epollfd);*/
	for(;;){
		sigset_t set;
		sigemptyset(&set);
		sigprocmask(SIG_SETMASK,&set,NULL);
		sigsuspend(&set);
	}
}
processpool*processpool::pool=NULL;
