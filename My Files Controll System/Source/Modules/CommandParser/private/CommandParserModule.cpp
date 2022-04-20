#include "pch.h"

void CommandParserModule::StartupModule()
{
}

void CommandParserModule::Update()
{
}

void CommandParserModule::ParseCommand(std::string& Command)
{
	std::string buf;
	std::vector<std::string> parsed;
	bool Quotes = false;

	for (size_t i = 0; i <= Command.length(); i++)
	{
		if (Command[i] != ' ' && Command[i] != '\0' && !Quotes)
		{
			buf += Command[i];
			if (Command[i] == '\"')
			{
				Quotes = true;
			}
		}
		else if(Quotes)
		{
			buf += Command[i];
			if (Command[i] == '\"')
			{
				Quotes = false;
			}
		}
		else
		{
			parsed.push_back(buf);
			buf.clear();
		}
	}
	if (parsed.size() == 0)
	{
		return;
	}
	if(parsed[0] != "monolit")
	{
		std::cout << "Invalit command" << std::endl;
		return;
	}
	auto Type = ParseCommandType(parsed[1]);
	
	std::set<CommandArguments> Args;

	switch (Type)
	{
	case CommandType::Error: {std::cout << "Invalit command" << std::endl; return; } break;
	case CommandType::AddIgnoredFile:
	{
		if (parsed.size() < 3)
		{
			std::cout << "Invalit path" << std::endl; return;
		}
		if (parsed.size() > 3)
		{
			Args = GetCommandArguments(std::set<std::string>(parsed.begin() + 3, parsed.end()));
		}
		auto Path = GetFileToIgnore(parsed[2]);
		if (Path != "")
		{
			GC->XML->AddFileToIgnore(Path);
		}
	}break;
	case CommandType::AddIgnoredDirectory: {
		if (parsed.size() < 3)
		{
			std::cout << "Invalit path" << std::endl; return;
		}
		if (parsed.size() > 3)
		{
			Args = GetCommandArguments(std::set<std::string>(parsed.begin() + 3, parsed.end()));
		}
		auto Path = GetDirectoryToIgnore(parsed[2]);
		if (Path != "")
		{
			GC->XML->AddDirectoryToIgnore(Path);
		}
	} break;
	case CommandType::Init:
	{
		if (parsed.size() < 5)
		{
			std::cout << "Invalit syntax" << std::endl; return;
		}
		Args = GetCommandArguments(std::set<std::string>(parsed.begin() + 4, parsed.begin() + 4));
		auto RepositoryPath = GetInitialRepositoryPath(parsed[2]);
		auto RepositoryName = GetInitialRepositoryName(parsed[4]);
		GC->System->InitNewRepository(RepositoryPath, RepositoryName);
	}break;
	}

	std::cout << std::endl;
}

CommandType CommandParserModule::ParseCommandType(std::string& Command)
{
	if(Command == "addignoreddir" || Command == "aid")
	{ 
		return CommandType::AddIgnoredDirectory;
	}
	if (Command == "addignoredfile" || Command == "aif")
	{
		return CommandType::AddIgnoredFile;
	}
	if (Command == "init" || Command == "i")
	{
		return CommandType::Init;
	}
	return CommandType::Error;
}

std::filesystem::path CommandParserModule::GetDirectoryToIgnore(std::string& Command)
{
	std::cout << Command << std::endl;
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	if (GC->Files->CheckDirectory(Command))
	{
		return Command;
	}
	else
	{
		std::cout << "Invalid path" << std::endl;
		return std::filesystem::path();
	}
}

std::filesystem::path CommandParserModule::GetFileToIgnore(std::string& Command)
{
	std::cout << Command << std::endl;
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	if (GC->Files->CheckFile(Command))
	{
		return Command;
	}
	else
	{
		std::cout << "Invalid path" << std::endl;
		return std::filesystem::path();
	}
}

std::filesystem::path CommandParserModule::GetInitialRepositoryPath(std::string& Command)
{
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	std::cout << Command << std::endl;
	if (GC->Files->CheckDirectory(Command))
	{
		return Command;
	}
	else
	{
		std::cout << "Invalid path" << std::endl;
		return std::filesystem::path();
	}
}

std::string CommandParserModule::GetInitialRepositoryName(std::string& Command)
{
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	std::cout << Command << std::endl;
	return Command;
}

std::set<CommandArguments> CommandParserModule::GetCommandArguments(std::set<std::string> Arguments)
{
	std::set<CommandArguments> result;
	if (Arguments.contains("-m"))
	{
		result.insert(CommandArguments::Mod);
	}
	if (Arguments.contains("-f") || Arguments.contains("-force"))
	{
		result.insert(CommandArguments::Force);
	}
	if (Arguments.contains("-n") || Arguments.contains("-name"))
	{
		result.insert(CommandArguments::Name);
	}
	return std::set<CommandArguments>();
}
