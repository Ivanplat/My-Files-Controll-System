#pragma once

#include "Modules/IBaseModule.h"
#include <vector>

class CommandParserModule : public IBaseModule
{
public:
	explicit CommandParserModule() noexcept : Super() {}
	virtual ~CommandParserModule() noexcept {}
public:
	void ParseConsoleCommand(std::string consoleCommand);
private:
	std::vector<std::string> ConsoleCommandToStringVector(std::string consoleCommand);
};