#pragma once

#include "Core/IBaseComponent.h"
#include "FileControll/public/FileControll.h"
#include "FileControll/public/XMLComponent.h"
#include "Internet/public/InternetComponent.h"
#include "CommandParser/public/CommandParserComponent.h"
#include "SystemComponent.h"
#include "Logger.h"

class GarbageCollector
{
public:
	static GarbageCollector* Instance();
public:
	std::unique_ptr<XMLComponent> XML;
	std::unique_ptr<FileControllComponent> Files;
	std::unique_ptr<InternetComponent> Internet;
	std::unique_ptr<CommandParserComponent> CommandParser;
	std::unique_ptr<SystemComponent> System;
	Logger* Log;
private:
	explicit GarbageCollector() noexcept;
	virtual ~GarbageCollector() noexcept;
};
