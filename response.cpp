#include<cstring>
#include"response.h"
response::response(){
	body=new char[4096];
	memset(body,'\0',4096*sizeof(char));
}
response::~response(){
	delete[]body;
}
