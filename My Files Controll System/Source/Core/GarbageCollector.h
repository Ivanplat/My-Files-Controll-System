#pragma once

#include <memory>
#include <cassert>
#include <vector>

#include "Core/IBaseComponent.h"
#include "FileControll/public/FileControll.h"
#include "FileControll/public/XMLComponent.h"
#include "Internet/public/InternetComponent.h"
#include "CommandParser/public/CommandParserComponent.h"

class GarbageCollector
{
public:
	static GarbageCollector* Instance();
public:
	std::unique_ptr<XMLComponent> XML;
	std::unique_ptr<FileControllComponent> Files;
	std::unique_ptr<InternetComponent> Internet;
	std::unique_ptr<CommandParserComponent> CommandParser;
private:
	explicit GarbageCollector() noexcept;
	virtual ~GarbageCollector() noexcept;
};