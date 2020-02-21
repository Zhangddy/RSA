#include "rsa.h"
#include <iostream>

using::std::cout;
using::std::cin;
using::std::endl;

void test()
{
	RSA rsa;
	
	rsa.ecrept("test.txt", "test.ecrept.txt");
	rsa.decrept("test.ecrept.txt", "test.decrept.txt");

}

int main()
{
	/*RSA rsa;
	rsa.initKeys();
	Key key = rsa.getKeys();

	DataType original = 2;
	DataType decout = rsa.ecrept(original, key._ekey, key._pkey);
	cout << "ekey: " << key._ekey << "dkey: " << key._dkey << "pkey: " << key._pkey << endl;
	cout << "original: " << original << endl;
	cout << "ecrept: " << decout << endl;
	cout << "decrpet: " << rsa.decrept(decout, key._dkey, key._pkey) << endl;*/
	test();
	return 0;
}