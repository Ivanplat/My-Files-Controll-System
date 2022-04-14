#include "Core/GarbageCollector.h"

GarbageCollector* GarbageCollector::Instance()
{
	static GarbageCollector gc;
	return &gc;
}

GarbageCollector::GarbageCollector() noexcept
{
	XML = std::unique_ptr<XMLComponent>(new XMLComponent("XMLComponent"));
	Files = std::unique_ptr<FileControllComponent>(new FileControllComponent("FileControll"));
	Internet = std::unique_ptr<InternetComponent>(new InternetComponent("InternetComponent"));
}

GarbageCollector::~GarbageCollector() noexcept
{
	XML.release();
	Files.release();
	Internet.release();
}
