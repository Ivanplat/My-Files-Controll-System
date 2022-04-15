#pragma once

#include "Modules/IBaseModule.h"

#include <filesystem>
#include <set>


enum class CommandType
{
	Error = -1,
	AddIgnoredFile = 0,
	AddIgnoredDirectory = 1
};

enum class CommandArguments
{
	Mod = 0,
	Force = 1
};



class CommandParserModule : public virtual IBaseModule
{
public:
	explicit CommandParserModule(std::string ComponentName) noexcept : Super(ComponentName) {}
public:
	virtual void StartupModule() override;
	virtual void Update() override;
public:
	virtual void ParseCommand(std::string& Command);
	virtual CommandType ParseCommandType(std::string& Command);
	virtual std::filesystem::path GetDirectoryToIgnore(std::string& Command);
	virtual std::set<CommandArguments> GetCommandArguments(std::set<std::string> Arguments);
};