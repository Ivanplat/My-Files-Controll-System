#include "pch.h"

int main()
{
	GarbageCollector::Instance();
	GC->Server->StartupModule();
	GC->Server->ShutdownModule();

	return 0;
}