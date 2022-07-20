#include "pch.h"

Logger* Logger::Instance()
{
	static Logger lg;
	return &lg;
}

void Logger::PrintToLog(std::string LogString)
{
}

void Logger::CreateLogFile()
{
}
