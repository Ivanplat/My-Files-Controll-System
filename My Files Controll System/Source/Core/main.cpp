#include "pch.h"

#include "SHA256/SHA256.h"
#include "AES/AES.h"

#include <stdarg.h>
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

	//GC->XML->StartupModule();

	GC->System->StartupModule();

	/*while (true)
	{
		std::cout << ">$ ";
		std::string input;
		std::getline(std::cin, input);
		GC->CommandParser->ParseCommand(input);
	}*/

	//std::cout<<GC->Files->HashOfBinaryFile("E:\\Unreal Engine\\UE_4.26\\Engine\\Binaries\\Win64\\UE4Editor.exe");
	while (true)
	{
		std::string buf;
		std::cin >> buf;
		if (buf == "0")
		{
			GC->System->ShutdownModule();
			break;
		}
	}

	return 0;
}