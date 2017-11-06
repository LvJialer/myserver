#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include"module.h"
#include"response.h"
#include"chartoHTML.h"
using namespace std;
module*process_module;
extern "C" void getprocess(response*res){
	FILE*fd=popen("ps -u","r");
	if(fd==NULL){cout<<"popen error"<<endl;exit(1);}
	char c[4096];for(int i=0;i<4096;i++){c[i]='\0';}
	fread(c,4096,1,fd);
	chartoHTML(res->body,c,4096);
	pclose(fd);
}
extern "C" module*hook(){
	process_module=new module("process",getprocess);
	return process_module;
}
