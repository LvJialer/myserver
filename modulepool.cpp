#include "modulepool.h"
#include <cstdio>
#include <cstring>
#include <dlfcn.h>
modulepool::modulepool(){
	load();
}
void modulepool::load(){
	FILE*module_conf=fopen("module.conf","r");
	if(module_conf==NULL)return;
	module_num=0;
	char buf[100];
	while(fgets(buf,sizeof(buf),module_conf)){
		if(buf[0]!='#'){
			module_num++;
		}
	}
	fseek(module_conf,0,SEEK_SET);
	modules=new module*[module_num];
	for(int i=0;i<module_num;){
		fgets(buf,sizeof(buf),module_conf);
		if(buf[0]=='#')continue;
		char add[20];
		char temp[20]="./";
		char filename[20];
		sscanf(buf,"%s %s\n",add,filename);
		if(strcmp(add,"add")!=0)return;
		void*handle=dlopen(strcat(temp,filename),RTLD_NOW);
		module*(*hook)()=(module*(*)())dlsym(handle,"hook");
		modules[i]=hook();
		modules[i]->handle=handle;
		i++;		
	}
	fclose(module_conf);
}
void modulepool::unload(){
    for(int i=0;i<module_num;i++){
        dlclose(modules[i]->handle);
        delete modules[i];
    }
    delete[]modules;
}
void modulepool::reload(){
	unload();
	load();
}
modulepool::~modulepool(){
	unload();
    delete modulepools;
}
modulepool*modulepool::get(){
	if(modulepools==NULL){
		modulepools=new modulepool();
	}
	return modulepools;
}
modulepool*modulepool::modulepools=NULL;