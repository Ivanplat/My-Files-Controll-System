#include "pch.h"
#include "CommandParserModule.h"

void CommandParserModule::ParseConsoleCommand(std::string consoleCommand)
{
	auto CommandVector = ConsoleCommandToStringVector(consoleCommand);

}

std::vector<std::string> CommandParserModule::ConsoleCommandToStringVector(std::string consoleCommand)
{
	std::vector<std::string> result;
	std::string tempString = "";
	for (auto it = consoleCommand.begin(); it != consoleCommand.end(); it++)
	{
		if (*it == ' ' || it == consoleCommand.end() - 1)
		{
			result.push_back(tempString);
			tempString.clear();
		}
		tempString += *it;
	}
	return result;
}
