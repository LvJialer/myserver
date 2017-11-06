#ifndef CHARTOHTML_H
#define CHARTOHTML_H

#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<iostream>
#include<cstdlib>
int chartoHTML(char* dest,char* src,int filelength)
{	
	
	strcat(dest,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\r\n<html>\r\n<body>\r\n");	
	strcat(dest,"<table><tr><td>");
	for(int i=0;i<filelength-1;i++)
	{
		if(src[i]==32)
		{
			strcat(dest,"</td><td>");
			while(src[i]==32&&src[i+1]==32)
				{
					i++;
				}
		}
		else if(src[i]=='\n'&&src[i+1]!='\0')
		{
			strcat(dest,"</td></tr><tr><td>");
		}
		else if(src[i]=='\n'&&src[i+1]=='\0')
		{
			strcat(dest,"</td></tr></table>");
		}
		else
		{
			int j=0;
			while(dest[j]!='\0')
			{j++;}
			dest[j]=src[i];
		}
	}
	strcat(dest,"</body>\r\n</html>\r\n");
	return 0;
}
#endif