#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
#include"module.h"
#include"response.h"
using namespace std;
module*disk_module;
extern "C" void getdisk(response*res){
	FILE*fd=popen("df -lh","r");
	if(fd==NULL){cout<<"popen error"<<endl;exit(1);}
	char c[600];for(int i=0;i<600;i++){c[i]='\0';}
	fread(c,600,1,fd);
	strcat(res->body,"<html>\r\n<body>\r\n");
	strcat(res->body,"<br><b>The state of the hard disk is:</b><br>");
	strcat(res->body,"<table><tr><td>");
	for(int i=0;i<599;i++){
		if(c[i]==32){strcat(res->body,"</td><td>");while(c[i]==32&&c[i+1]==32){i++;}}
		else if(c[i]=='\n'&&c[i+1]!='\0'){strcat(res->body,"</td></tr><tr><td>");}
		else if(c[i]=='\n'&&c[i+1]=='\0'){strcat(res->body,"</td></tr></table>");}
		else{int j=0;while(res->body[j]!='\0'){j++;}res->body[j]=c[i];}
	}
	strcat(res->body,"</body>\r\n</html>\r\n");
	pclose(fd);
}
extern "C" module*hook(){
	disk_module=new module("disk",getdisk);
	return disk_module;
}
