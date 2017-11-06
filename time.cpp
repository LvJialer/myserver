#include<cstdio>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include"module.h"
#include"response.h"
#include"chartoHTML.h"
using namespace std;
module*time_module;
extern "C" void gettime(response*res){
	FILE*fd=popen("uname -v","r");
	if(fd==NULL){cout<<"popen error"<<endl;exit(1);}
	char c[4096];for(int i=0;i<4096;i++){c[i]='\0';}
	fread(c,4096,1,fd);
	chartoHTML(res->body,c,4096);
	pclose(fd);
}
extern "C" module*hook(){
	time_module=new module("time",gettime);
	return time_module;
}
