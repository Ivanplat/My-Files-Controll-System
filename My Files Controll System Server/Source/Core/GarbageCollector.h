#pragma once

#include <memory>

#include "Core/Logger.h"
#include "Modules/FilesModule/public/FilesModule.h"
#include "Modules/ServerModule/public/ServerModule.h"
#include "Modules/Authentication/public/AuthenticationModule.h"

class GarbageCollector
{
public:
	static GarbageCollector* Instance();
public:
	Logger* Log;
	std::unique_ptr<FilesModule> Files;
	std::unique_ptr<ServerModule> Server;
	std::unique_ptr<AuthenticationModule> Authentication;
private:
	explicit GarbageCollector() noexcept;
	virtual ~GarbageCollector() noexcept;
};