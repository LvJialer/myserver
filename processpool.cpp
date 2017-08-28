#include"processpool.h"
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/wait.h>
#include<iostream>
#include<string>
#include<dlfcn.h>
#include<cstring>
using namespace std;
processpool::processpool(int listenfd,int processnum):listenfd(listenfd),processnum(processnum){
	subprocess=new process[processnum];
	for(int i=0;i<processnum;i++){
		if(pipe(subprocess[i].pipefd)<0){cout<<"pipe num "<<"i"<<" error"<<endl;}
		subprocess[i].mypid=fork();
		if(subprocess[i].mypid<0){cout<<"fork num "<<"i"<<" error"<<endl;}
		if(subprocess[i].mypid>0){close(subprocess[i].pipefd[0]);continue;}
		else{close(subprocess[i].pipefd[1]);idx=i;break;}
	}
}
processpool*processpool::get(int listenfd){
	if(pool==nullptr){
		pool=new processpool(listenfd);
	}
	return pool;
}
processpool::~processpool(){
	delete[]subprocess;
}
void processpool::run(){
	if(idx!=-1){runchild();return;}
	runparent();
}
void processpool::runchild(){
	while(1){
		int b;int*buf=&b;
		read(subprocess[idx].pipefd[0],buf,sizeof(int));
		if(*buf==idx){
			struct sockaddr_in clientaddr;
			unsigned int len=sizeof(clientaddr);
			int newsock=accept(listenfd,reinterpret_cast<sockaddr*>(&clientaddr),&len);
			if(newsock<0){cout<<"accept error"<<endl;}
			else{cout<<"Accept success and child process No."<<idx<<" is working."<<endl;}
			cout<<"Please choose the orders"<<endl;
			string choose;
			cin>>choose;
			char sen[4096];for(int i=0;i<4096;i++){sen[i]='\0';}
			char p[]="HTTP/1.0 200 OK\r\nServer:myserver\r\nContent-length:4096\r\nContent-type:text/html\r\n\r\n<html>\r\n<body>\r\n";
			strcat(sen,p);
			if(choose.find('a')!=string::npos){
				void*handle;
				void(*time)(char*);
				handle=dlopen("./a_time.so",RTLD_LAZY);
				if(handle==nullptr){cout<<"dlfcn error"<<endl;}
				*(void**)(&time)=dlsym(handle,"gettime");
				char buff[50];for(int i=0;i<50;i++){buff[i]='\0';}
				(*time)(buff);
				char ma[]="<br><b>The time of the server is:</b><br>";
				strcat(sen,ma);
				strcat(sen,buff);
				dlclose(handle);
			}
			if(choose.find('b')!=string::npos){
				void*handle;
				void(*version)(char*);
				handle=dlopen("./b_version.so",RTLD_LAZY);
				if(handle==nullptr){cout<<"dlfcn error"<<endl;}
				*(void**)(&version)=dlsym(handle,"getversion");
				char*buff=new char[50];for(int i=0;i<50;i++){buff[i]='\0';}
				(*version)(buff);
				strcat(sen,"<br><b>The version of the server is:</b><br>");
				strcat(sen,buff);
				delete[]buff;
				dlclose(handle);
			}
			if(choose.find('c')!=string::npos){
				void*handle;
				void(*disk)(char*);
				handle=dlopen("./c_disk.so",RTLD_LAZY);
				if(handle==nullptr){cout<<"dlfcn error"<<endl;}
				*(void**)(&disk)=dlsym(handle,"getdisk");
				char*buff=new char[1300];for(int i=0;i<1300;i++){buff[i]='\0';}
				(*disk)(buff);
				strcat(sen,"<br><b>The state of the hard disk is:</b><br>");
				strcat(sen,buff);
				delete[]buff;
				dlclose(handle);
			}
			if(choose.find('d')!=string::npos){
				void*handle;
				void(*process)(char*);
				handle=dlopen("./d_process.so",RTLD_LAZY);
				if(handle==nullptr){cout<<"dlfcn error"<<endl;}
				*(void**)(&process)=dlsym(handle,"getprocess");
				char*buff=new char[2000];for(int i=0;i<2000;i++){buff[i]='\0';}
				(*process)(buff);
				strcat(sen,"<br><b>The Processes of the server is:</b><br>");
				strcat(sen,buff);
				delete[]buff;
				dlclose(handle);
			}
			char q[]="</body>\r\n</html>\r\n";
			strcat(sen,q);
			int i=0;
			for(;i<4096&&sen[i]!='\0';i++){}
			char*se=new char[i];
			for(int j=0;j<i;j++){se[j]=sen[j];}
			send(newsock,se,i,0);
			delete[]se;
		}
	}
}
void processpool::runparent(){
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
	close(epollfd);
}
processpool*processpool::pool=nullptr;
