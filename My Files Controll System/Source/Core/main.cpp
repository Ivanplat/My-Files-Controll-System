#include "pch.h"
#include <stdarg.h>

#include "IBaseComponent.h"
#include "FileControll/public/FileControll.h"
#include "FileControll/public/XMLComponent.h"
#include "Internet/public/InternetComponent.h"

#include "GarbageCollector.h"
#include "Logger.h"

#include "SHA256/SHA256.h"
#include "AES/AES.h"

#include <iostream>
#include <random>

std::string KeyGenerator()
{
	std::string result;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(97, 122);
	for (size_t i = 0; i < 32; i++)
	{
		result += static_cast<char>(dist(gen));
	}
	return result;
}
std::string VectorGenerator()
{
	std::string result;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(97, 122);
	for (size_t i = 0; i < 16; i++)
	{
		result += static_cast<char>(dist(gen));
	}
	return result;
}

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

	auto iv = VectorGenerator();
	AES aes(AESKeyLength::AES_256);
	std::string text = "Kushat";
	auto d = aes.SetRightStringLengh(text);
	auto plain = aes.StringToUCharPtr(text);

	unsigned char key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

	unsigned char* out = aes.EncryptECB(plain, 16, key);
	auto dec = aes.DecryptECB(out, 16, key);

	std::cout << plain << std::endl << std::endl;
	std::cout << out << std::endl << std::endl;
	std::cout << dec << std::endl << std::endl;

	auto res = aes.ToString(dec);

	aes.RemoveUnusedChars(res, d);
	std::cout << res << std::endl;

	delete[] out;

	return 0;
}