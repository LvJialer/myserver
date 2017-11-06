#ifndef MODULE_H
#define MODULE_H

#include"response.h"
class module{
public:
	char name[15];
	void*handle;
	void(*command)(response*res);
	module();
	module(const char*name,void(*command)(response*));
};
#endif
