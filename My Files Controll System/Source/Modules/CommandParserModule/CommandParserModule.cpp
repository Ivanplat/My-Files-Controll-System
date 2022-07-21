#include "pch.h"
#include "CommandParserModule.h"
#include "Modules/FilesControlModule/FilesControlModule.h"
#include "Modules/SystemModule/SystemModule.h"
#include <filesystem>

void CommandParserModule::ParseConsoleCommand(std::string consoleCommand)
{
	auto commandVector = ConsoleCommandToStringVector(consoleCommand);
	if (commandVector.size() == 0)
	{
		throw CommandExeption("INVALID SYNTAXIS (AGRUMENTS COUNT IS 0)",
			  CommandExeption::ECommandExeptionType::InvalidSyntaxis);
	}
	std::string command = commandVector[0];
	auto commandType = GetCommandTypeByString(command);
	switch (commandType)
	{
	case ECommandType::Initialize:
	{
		auto path = commandVector[1];
		switch (GC->System->CouldInitilializeProject(path))
		{
		case COULD_INITIALIZE_NEW_PROJECT:
		{
			GC->System->InitializeNewProject(path);
		}break;
		case PROJECT_IS_ALREADY_INITIALIZED:
		{
			throw SystemModuleException("PROJECT IS ALREADY INITIALIZED");
		}break;
		case FOLDER_DOES_NOT_EXIST_VALUE:
		{
			throw CommandExeption("UNVALID FOLDER PATH",
				  CommandExeption::ECommandExeptionType::InvalidArguments);
		}break;
		}
	}break;
	case ECommandType::Select:
	{
		auto path = commandVector[1];
		GC->System->SelectProject(path);
	}break;
	case ECommandType::Delete:
	{
		auto path = commandVector[1];
		GC->System->DeleteProject(path);
	}break;
	case ECommandType::NewBranch:
	{
		auto branchName = commandVector[1];
		GC->System->AddNewBranch(branchName);
	}break;
	case ECommandType::AddIgnoredFile:
	{

	}break;
	case ECommandType::AddIgnoredFolder:
	{

	}break;
	case ECommandType::Commit:
	{
		GC->System->Commit();
	}break;
	case ECommandType::Push:
	{

	}break;
	case ECommandType::Pull:
	{

	}break;
	case ECommandType::Check:
	{
		GC->FilesControl->Check();
	}break;
	case ECommandType::Unknown:
	{
		throw CommandExeption("UNKNOWN COMMAND",
			 CommandExeption::ECommandExeptionType::InvalidArguments);
	}break;
	}
}

std::vector<std::string> CommandParserModule::ConsoleCommandToStringVector(std::string consoleCommand)
{
	std::vector<std::string> result;
	std::string tempString = "";
	for (auto it = consoleCommand.begin(); it != consoleCommand.end(); it++)
	{
		if (*it == ' ')
		{
			result.push_back(tempString);
			tempString.clear();
			if (it != consoleCommand.end() - 1)
			{
				it++;
			}
		}
		if (it == consoleCommand.end() - 1)
		{
			tempString += *it;
			result.push_back(tempString);
			tempString.clear();
		}
		tempString += *it;
	}
	return result;
}

CommandParserModule::ECommandType CommandParserModule::GetCommandTypeByString(std::string command)
{
	if (command == "i"    || command == "init" || command == "initialize")  return ECommandType::Initialize;
	if (command == "d"	  || command == "del"  || command == "delete")      return ECommandType::Delete;
	if (command == "s"    || command == "select")                           return ECommandType::Select;
	if (command == "afi"  || command == "addfileignore")					return ECommandType::AddIgnoredFile;
	if (command == "afli" || command == "addfolderignore")					return ECommandType::AddIgnoredFolder;
	if (command == "check")													return ECommandType::Check;
	if (command == "commit")												return ECommandType::Commit;
	if (command == "push")													return ECommandType::Push;
	if (command == "pull")													return ECommandType::Pull;
	if (command == "newbranch")												return ECommandType::NewBranch;
	return ECommandType::Unknown;
}
