#include<cstring>
#include<cstdio>
#include<dlfcn.h>
#include"module.h"
#include"response.h"
module**modules;
int module_num;
module::module(){
	memset(this->name,'\0',15);
}
module::module(const char*name,void(*command)(response*)){
	memset(this->name,'\0',15);
	strcpy(this->name,name);
	this->command=command;
}
void load_modules(){
	FILE*module_conf=fopen("module.conf","r");
	if(module_conf==NULL)return;
	module_num=0;
	char buf[100];
	while(fgets(buf,sizeof(buf),module_conf))module_num++;
	fseek(module_conf,0,SEEK_SET);
	modules=new module*[module_num];
	for(int i=0;i<module_num;i++){
		char add[20];
		char temp[20]="./";
		char filename[20];
		fscanf(module_conf,"%s",add);
		if(strcmp(add,"add")!=0)return;
		fscanf(module_conf,"%s\n",filename);
		void*handle=dlopen(strcat(temp,filename),RTLD_NOW);
		module*(*hook)()=(module*(*)())dlsym(handle,"hook");
		modules[i]=hook();
		modules[i]->handle=handle;
	}
	fclose(module_conf);
}

