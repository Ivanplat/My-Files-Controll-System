#include "pch.h"
#include "GarbageCollector.h"

GarbageCollector* GarbageCollector::Instance()
{
	static GarbageCollector gc;
	return &gc;
}

GarbageCollector::GarbageCollector() noexcept
{
	Log = Logger::Instance();
	Files = std::unique_ptr<FilesModule>(new FilesModule("FilesModule"));
}

GarbageCollector::~GarbageCollector() noexcept
{

}