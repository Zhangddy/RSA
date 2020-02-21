#include "rsa.h"
#include <iostream>
#include <ctime>
#include <fstream>

RSA::RSA()
{
	initKeys();
}

void RSA::ecrept(const char* filein, const char* fileout)
{
	std::ifstream fin(filein, std::ifstream::binary);
	std::ofstream fout(fileout, std::ofstream::binary);
	if (!fin.is_open())
	{
		perror("ecrept: input file open failed!");
		return;
	}

	char* bufferIn = new char[BUFFER_SIZE];
	DataType* bufferOut = new DataType[BUFFER_SIZE];

	while (!fin.eof())
	{
		fin.read(bufferIn, BUFFER_SIZE);
		int curNum = fin.gcount(); // 返回最近一次读取的字节数
		for (int i = 0; i < curNum; i++)
		{
			bufferOut[i] = ecrept((DataType)bufferIn[i], _key._ekey, _key._pkey);
		}
		fout.write((char*)bufferOut, curNum * sizeof(DataType));
	}
	fin.close();
	fout.close();
	delete[] bufferIn;
	delete[] bufferOut;
}

void RSA::decrept(const char* filein, const char* fileout)
{
	std::ifstream fin(filein, std::ifstream::binary);
	std::ofstream fout(fileout, std::ofstream::binary);
	if (!fin.is_open())
	{
		perror("decrept: input file open failed!");
		return;
	}

	DataType* bufferIn = new DataType[BUFFER_SIZE];
	char* bufferOut = new char[BUFFER_SIZE];

	while (!fin.eof())
	{
		fin.read((char*)bufferIn, BUFFER_SIZE * sizeof(DataType));
		int num = fin.gcount(); // 返回最近一次读取的字节数
		num /= sizeof(DataType);
		for (int i = 0; i < num; i++)
		{
			bufferOut[i] = decrept(bufferIn[i], _key._dkey, _key._pkey);
		}
		fout.write(bufferOut, num);
	}
	fin.close();
	fout.close();
	delete[] bufferIn;
	delete[] bufferOut;
}

void RSA::initKeys()
{
	DataType prime1 = getPrime();
	DataType prime2 = getPrime();
	while (prime1 == prime2)
	{
		prime2 = getPrime();
	}

	DataType orla = getOrla(prime1, prime2);
	_key._pkey = getPkey(prime1, prime2);
	_key._ekey = getEkey(orla);
	_key._dkey = getDkey(_key._ekey, orla);
}

Key RSA::getKeys()
{
	return _key;
}

DataType RSA::ecrept(DataType data, DataType ekey, DataType pkey)
{
	// 直接运算幂次方时间复杂度过大
	// 使用快速幂取模运算
	DataType Ai = data;
	DataType msgE = 1;

	while (ekey)
	{
		if (ekey & 1)
		{
			msgE = (msgE * Ai) % pkey;
		}
		ekey >>= 1;
		Ai = (Ai * Ai) % pkey;
	}
	return msgE;
}

DataType RSA::decrept(DataType data, DataType dkey, DataType pkey)
{
	return ecrept(data, dkey, pkey);
}

DataType RSA::getPrime()
{
	srand(time(NULL));
	DataType prime;
	while (true)
	{
		prime = rand() % 100 + 2;
		if (isPrime(prime))
		{
			break;
		}
	}
	return prime;
}

bool RSA::isPrime(DataType data)
{
	if (data <= 0)
	{
		return false;
	}
	for (int i = 2; i <= sqrt(data); i++)
	{
		if (data % i == 0)
			return false;
	}
	return true;
}

DataType RSA::getPkey(DataType prime1, DataType prime2)
{
	return prime1 * prime2;
}

DataType RSA::getOrla(DataType prime1, DataType prime2)
{
	return (prime1 - 1) * (prime2 - 1);
}

DataType RSA::getEkey(DataType orla)
{
	srand(time(NULL));
	DataType ekey;
	while (true)
	{
		ekey = rand() % orla;
		if (ekey > 1 && getGcd(ekey, orla) == 1)
		{
			break;
		}
	}
	return ekey;
}

DataType RSA::getDkey(DataType ekey, DataType orla)
{
	DataType dkey = orla / ekey;
	while (true)
	{
		if ((dkey * ekey) % orla == 1)
		{
			break;
		}
		++dkey;
	}
	return dkey;
}

DataType RSA::getGcd(DataType data1, DataType data2)
{
	// 辗转相除法
	DataType residual = 0; // 余数
	while (residual = data1 % data2)
	{
		data1 = data2;
		data2 = residual;
	}
	return data2;
}