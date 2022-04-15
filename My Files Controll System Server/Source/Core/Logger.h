#pragma once
#include <string>

class Logger
{
public:
	static Logger* Instance();
public:
	virtual void PrintToConsole(std::string LogString);
private:
	explicit Logger() noexcept;
	virtual ~Logger() noexcept;
};