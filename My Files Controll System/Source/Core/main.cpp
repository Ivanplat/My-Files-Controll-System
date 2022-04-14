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

	//GC->XML->AddDirectoryToIgnore("1232\\asda\\asdavv");
	//GC->XML->AddDirectoryToIgnore("dfgddfg\\dfgd\\vbvbv");
	//GC->XML->AddDirectoryToIgnore("vvvv\\aaaaa\\ddddd");

	//GC->XML->AddFileToIgnore("ABOBA.txt", "423432");
	//GC->XML->AddFileToIgnore("ABOBA2.txt", "bcvbf");

	GC->Internet->StartupModule();
	GC->Files->StartupModule();


	return 0;
}