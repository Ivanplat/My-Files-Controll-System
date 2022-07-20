#include "pch.h"
#include "Core/GarbageCollector.h"
#include "Modules/FilesControlModule/FilesControlModule.h"
#include "Modules/SystemModule/SystemModule.h"
#include "Modules/XMLModule/XMLModule.h"
#include "Logger.h"


GarbageCollector* GarbageCollector::Instance()
{
	static GarbageCollector gc;
	return &gc;
}

GarbageCollector::GarbageCollector() noexcept
{
	FilesControl = std::make_unique<FilesControlModule>();
	System = std::make_unique<SystemModule>();
	XML = std::make_unique<XMLModule>();
	Log = Logger::Instance();
}

GarbageCollector::~GarbageCollector() noexcept
{
	FilesControl.release();
	System.release();
}
