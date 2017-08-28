#include<cstdio>
#include<unistd.h>
#include<iostream>
using namespace std;
extern "C" void getversion(char*t){
	FILE*fd=popen("lsb_release -d","r");
	if(fd==nullptr){cout<<"popen error"<<endl;exit(1);}
	fread(t,50,1,fd);
	pclose(fd);
}
