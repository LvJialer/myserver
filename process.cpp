#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
using namespace std;
extern "C" void getprocess(char*t){
	FILE*fd=popen("ps -u","r");
	if(fd==NULL){cout<<"popen error"<<endl;exit(1);}
	char c[4096];for(int i=0;i<4096;i++){c[i]='\0';}
	fread(c,4096,1,fd);
	strcat(t,"<table><tr><td>");
	for(int i=0;i<4096;i++){
		if(c[i]==32){strcat(t,"</td><td>");while(c[i]==32&&c[i+1]==32){i++;}}
		else if(c[i]=='\n'&&c[i+1]!='\0'){strcat(t,"</td></tr><tr><td>");}
		else if(c[i]=='\n'&&c[i+1]=='\0'){strcat(t,"</td></tr></table>");}
		else{int j=0;while(t[j]!='\0'){j++;}t[j]=c[i];}
	}
	pclose(fd);
}
