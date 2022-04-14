#include "pch.h"
#include <stdarg.h>

#include "IBaseComponent.h"
#include "FileControll/public/FileControll.h"
#include "FileControll/public/XMLComponent.h"
#include "Internet/public/InternetComponent.h"

#include "GarbageCollector.h"
#include "Logger.h"



int main()
{
	setlocale(LC_ALL, "Russian");

	GC->Log->CreateLogFile();
	GC->XML->StartupModule();
	GC->Internet->StartupModule();
	GC->Files->StartupModule();

	std::string command;
	while (true)
	{
		std::cout << ">$ ";
		std::getline(std::cin, command);
		GC->CommandParser->ParseCommand(command);
	}

	return 0;
}