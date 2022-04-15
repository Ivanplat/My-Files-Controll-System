#include "pch.h"
#include "Logger.h"


Logger* Logger::Instance()
{
	static Logger lg;
	return &lg;
}

Logger::Logger() noexcept
{

}

Logger::~Logger() noexcept
{

}