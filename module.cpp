#include"module.h"
#include"response.h"
#include <cstring>
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

