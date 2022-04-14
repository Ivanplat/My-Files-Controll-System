#include "Core/GarbageCollector.h"

GarbageCollector* GarbageCollector::Instance()
{
	static GarbageCollector gc;
	return &gc;
}
