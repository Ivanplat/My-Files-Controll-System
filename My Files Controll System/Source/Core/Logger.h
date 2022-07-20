#pragma once
#include "pch.h"
#include <filesystem>

class Logger
{
public:
	static Logger* Instance();
	virtual void PrintToLog(std::string LogString);
	virtual void CreateLogFile();
protected:
	std::filesystem::path CurrentLogFilePath;
private:
	explicit Logger() noexcept = default;
	virtual ~Logger() noexcept = default;
};