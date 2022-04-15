#pragma once

#include "Modules/IBaseModule.h"
#include "Modules/FileControll/public/FileControllModule.h"
#include "Modules/FileControll/public/XMLModule.h"
#include "Modules/Client/public/ClientModule.h"
#include "Modules/CommandParser/public/CommandParserModule.h"
#include "Modules/System/public/SystemModule.h"
#include "Logger.h"

class GarbageCollector
{
public:
	static GarbageCollector* Instance();
public:
	std::unique_ptr<XMLModule> XML;
	std::unique_ptr<FileControllModule> Files;
	std::unique_ptr<ClientModule> Client;
	std::unique_ptr<CommandParserModule> CommandParser;
	std::unique_ptr<SystemModule> System;
	Logger* Log;
private:
	explicit GarbageCollector() noexcept;
	virtual ~GarbageCollector() noexcept;
};
