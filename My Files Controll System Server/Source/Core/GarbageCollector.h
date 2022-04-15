#pragma once

#include <memory>

#include "Core/Logger.h"
#include "Modules/FilesModule/public/FilesModule.h"

class GarbageCollector
{
public:
	static GarbageCollector* Instance();
public:
	Logger* Log;
	std::unique_ptr<FilesModule> Files;
private:
	explicit GarbageCollector() noexcept;
	virtual ~GarbageCollector() noexcept;
};