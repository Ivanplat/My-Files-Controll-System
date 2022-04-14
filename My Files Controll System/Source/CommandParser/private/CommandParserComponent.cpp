#include "pch.h"
#include "CommandParser/public/CommandParserComponent.h"

void CommandParserComponent::StartupModule()
{
}

void CommandParserComponent::Update()
{
}

void CommandParserComponent::ParseCommand(std::string& Command)
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
	if (parsed.size() > 3)
	{
		Args = GetCommandArguments(std::set<std::string>(parsed.begin() + 3, parsed.end()));
	}

	switch (Type)
	{
	case CommandType::Error: {std::cout << "Invalit command" << std::endl; return; } break;
	case CommandType::AddIgnoredFile: break;
	case CommandType::AddIgnoredDirectory: {
		if (parsed.size() < 3)
		{
			std::cout << "Invalit path" << std::endl; return;
		}
		auto Path = GetDirectoryToIgnore(parsed[2]);
		if (Path != "")
		{
			GC->XML->AddDirectoryToIgnore(Path);
		}
	} break;
	}

	std::cout << std::endl;
}

CommandType CommandParserComponent::ParseCommandType(std::string& Command)
{
	if(Command == "addignoreddir" || Command == "aid")
	{ 
		return CommandType::AddIgnoredDirectory;
	}
	if (Command == "addignoredfile" || Command == "aif")
	{
		return CommandType::AddIgnoredFile;
	}
	return CommandType::Error;
}

std::filesystem::path CommandParserComponent::GetDirectoryToIgnore(std::string& Command)
{
	std::cout << Command << std::endl;
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	Command.erase(std::remove(Command.begin(), Command.end(), '\"'));
	if (FileControllComponent::CheckDirectory(Command))
	{
		return Command;
	}
	else
	{
		std::cout << "Invalid path" << std::endl;
		return std::filesystem::path();
	}
}

std::set<CommandArguments> CommandParserComponent::GetCommandArguments(std::set<std::string> Arguments)
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
	return std::set<CommandArguments>();
}
