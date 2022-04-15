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
	Server = std::unique_ptr<ServerModule>(new ServerModule("ServerModule"));
	Authentication = std::unique_ptr<AuthenticationModule>(new AuthenticationModule("AuthenticationModule"));
}

GarbageCollector::~GarbageCollector() noexcept
{

}