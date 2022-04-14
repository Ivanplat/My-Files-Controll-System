#include <iostream>
#include <memory>
#include <stdarg.h>
#include <fstream>

#include "IBaseComponent.h"
#include "FileControll/public/FileControll.h"
#include "FileControll/public/XMLComponent.h"
#include "Internet/public/InternetComponent.h"

#include "GarbageCollector.h"



int main()
{
	setlocale(LC_ALL, "Russian");

	GarbageCollector* GC = GarbageCollector::Instance();
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