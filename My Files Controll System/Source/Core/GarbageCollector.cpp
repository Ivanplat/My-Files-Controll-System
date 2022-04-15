#include "pch.h"
#include "Core/GarbageCollector.h"

GarbageCollector* GarbageCollector::Instance()
{
	static GarbageCollector gc;
	return &gc;
}

GarbageCollector::GarbageCollector() noexcept
{
	XML = std::unique_ptr<XMLModule>(new XMLModule("XMLModule"));
	Files = std::unique_ptr<FileControllModule>(new FileControllModule("FileControllModule"));
	Client = std::unique_ptr<ClientModule>(new ClientModule("ClientModule"));
	CommandParser = std::unique_ptr<CommandParserModule>(new CommandParserModule("CommandParserModule"));
	System = std::unique_ptr<SystemModule>(new SystemModule("SystemModule"));
	Log = Logger::Instance();
}

GarbageCollector::~GarbageCollector() noexcept
{
	XML.release();
	Files.release();
	Client.release();
	CommandParser.release();
}
