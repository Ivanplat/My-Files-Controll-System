#pragma once

#include "Modules/IBaseModule.h"
#include <vector>

class CommandParserModuleExeption : public ModuleException
{
public:
	explicit CommandParserModuleExeption(const char* Message_) : ModuleException(Message_) {}
};

class CommandExeption : public CommandParserModuleExeption
{
public:
	enum class ECommandExeptionType
	{
		InvalidSyntaxis,
		InvalidArguments
	};
public:
	explicit CommandExeption(const char* Message_, ECommandExeptionType CommandExeptionType) : 
													CommandParserModuleExeption(Message_),
													CommandExeptionType_(CommandExeptionType) {}
public:
	const ECommandExeptionType& Type() const { return CommandExeptionType_; }
private:
	ECommandExeptionType CommandExeptionType_;
};

class CommandParserModule : public IBaseModule
{
public:
	enum class ECommandType
	{
		Initialize,
		Select,
		Delete,
		AddIgnoredFile,
		AddIgnoredFolder,
		Check,
		Commit,
		Push,
		Pull,
		NewBranch,
		Unknown
	};
public:
	explicit CommandParserModule() noexcept : Super() {}
	virtual ~CommandParserModule() noexcept {}
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
	virtual void Update() override {}
public:
	void ParseConsoleCommand(std::string consoleCommand);
private:
	std::vector<std::string> ConsoleCommandToStringVector(std::string consoleCommand);
	ECommandType GetCommandTypeByString(std::string command);
};