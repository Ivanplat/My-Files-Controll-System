#pragma once

#include "Modules/IBaseModule.h"
#include <memory>

class GarbageCollector
{
public:
	static GarbageCollector* Instance();
public:
	class Logger* Log;
	std::unique_ptr<class FilesControlModule> FilesControl;
	std::unique_ptr<class SystemModule> System;
	std::unique_ptr<class XMLModule> XML;
private:
	explicit GarbageCollector() noexcept;
	virtual ~GarbageCollector() noexcept;
};
