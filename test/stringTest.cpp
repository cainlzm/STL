#include "stringTest.h"
#include <iostream>
using std::cout;
using std::endl;
using namespace zm;
void stringTest()
{
	zm::string a;
	cout << a;
	zm::string b("abc");
    cout << b ;
	zm::string c(b);
	cout << c;
	
}