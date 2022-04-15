#include "pch.h"
#include "Logger.h"


Logger* Logger::Instance()
{
	static Logger lg;
	return &lg;
}

void Logger::PrintToConsole(std::string LogString)
{
	std::cout << LogString << std::endl;
}

Logger::Logger() noexcept
{

}

Logger::~Logger() noexcept
{

}