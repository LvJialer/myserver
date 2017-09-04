#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include"module.h"
#include"response.h"
using namespace std;
module*process_module;
extern "C" void getprocess(response*res){
	FILE*fd=popen("ps -u","r");
	if(fd==NULL){cout<<"popen error"<<endl;exit(1);}
	char c[4096];for(int i=0;i<4096;i++){c[i]='\0';}
	fread(c,4096,1,fd);
	strcat(res->body,"<html>\r\n<body>\r\n");
	strcat(res->body,"<br><b>The state of the hard disk is:</b><br>");
	strcat(res->body,"<table><tr><td>");
	for(int i=0;i<4096;i++){
		if(c[i]==32){strcat(res->body,"</td><td>");while(c[i]==32&&c[i+1]==32){i++;}}
		else if(c[i]=='\n'&&c[i+1]!='\0'){strcat(res->body,"</td></tr><tr><td>");}
		else if(c[i]=='\n'&&c[i+1]=='\0'){strcat(res->body,"</td></tr></table>");}
		else{int j=0;while(res->body[j]!='\0'){j++;}res->body[j]=c[i];}
	}
	strcat(res->body,"</body>\r\n</html>\r\n");
	pclose(fd);
}
extern "C" module*hook(){
	process_module=new module("process",getprocess);
	return process_module;
}
