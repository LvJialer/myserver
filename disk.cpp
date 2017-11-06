#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include"module.h"
#include"response.h"
#include"chartoHTML.h"
using namespace std;
module*disk_module;
extern "C" void getdisk(response*res){
	FILE*fd=popen("df -lh","r");
	if(fd==NULL){cout<<"popen error"<<endl;exit(1);}
	char c[4096];for(int i=0;i<4096;i++){c[i]='\0';}
	fread(c,4096,1,fd);
	chartoHTML(res->body,c,4096);
	pclose(fd);
}
extern "C" module*hook(){
	disk_module=new module("disk",getdisk);
	return disk_module;
}
