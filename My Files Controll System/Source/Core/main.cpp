#include "pch.h"
#include <stdarg.h>

#include "Modules/IBaseModule.h"
#include "Modules/FileControll/public/FileControllModule.h"
#include "Modules/FileControll/public/XMLModule.h"
#include "Modules/Client/public/ClientModule.h"

#include "GarbageCollector.h"
#include "Logger.h"

#include "SHA256/SHA256.h"
#include "AES/AES.h"

#include <iostream>
#include <random>

void AddLengh(std::string& str)
{
	auto d = str.length() % 16;
	if (d != 0)
	{
		auto k = 16 - str.length();
		for (size_t i = 0; i < k; i++)
		{
			str.push_back('-');
		}
	}
}

unsigned char* getRandomPlain(unsigned int length)
{
	unsigned char* plain = new unsigned char[length];
	for (unsigned int i = 0; i < length; i++) {
		plain[i] = rand() % 256;
	}

	return plain;

}

int main()
{
	setlocale(LC_ALL, "Russian");

	return 0;
}