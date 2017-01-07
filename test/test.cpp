#include<iostream>
#include "stringTest.h"
#include "zm_stl_construct.h"
int main()
{ 
	int a=0;
	int b = 2;
	std::cout << a << std::endl;
	zm::construct(&a,b);
	std::cout << a << std::endl;
	//stringTest();
	return 0;
}