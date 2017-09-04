#include<cstdio>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include"module.h"
#include"response.h"
using namespace std;
module*version_module;
extern "C" void getversion(response*res){
	FILE*fd=popen("lsb_release -d","r");
	if(fd==NULL){cout<<"popen error"<<endl;exit(1);}
	char buf[100];
	memset(buf,'\0',100*sizeof(char));
	fread(buf,100,1,fd);
	sprintf(res->body,"<html>\r\n<body>\r\n");
	sprintf(res->body,"%s<br><b>The version of the server is:</b><br>",res->body);
	sprintf(res->body,"%s%s",res->body,buf);
	sprintf(res->body,"%s</body>\r\n</html>\r\n",res->body);
	pclose(fd);
}
extern "C" module*hook(){
	version_module=new module("version",getversion);
	return version_module;
}
