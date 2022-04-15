#include "pch.h"

int main()
{
	GarbageCollector::Instance();
	GC->Server->StartupModule();


	return 0;
}