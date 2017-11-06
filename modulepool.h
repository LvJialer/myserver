#ifndef MODULEPOOL_H
#define MODULEPOOL_H

#include "module.h"
class modulepool{
private:
    modulepool();
    static modulepool*modulepools;
public:
    module**modules;
    int module_num;
	~modulepool();
	static modulepool*get();
};
#endif