#pragma once

#include "Modules/IBaseModule.h"
#include <filesystem>

class SystemModuleException : public ModuleException 
{
public:
	explicit SystemModuleException(const char* _Message) : ModuleException(_Message) {}
};
class ProjectInitializeExeption : public SystemModuleException 
{
public:
	enum class EInitializeExecptionType
	{
		NotInitialized,
		AlreadyInitialized,
		NotSelected,
		NotInitializedOrSelected
	};
private:
	EInitializeExecptionType InitializeExecptionType_;
public:
	explicit ProjectInitializeExeption(const char* _Message, EInitializeExecptionType InitializeExecptionType) :
																				SystemModuleException(_Message),
																				InitializeExecptionType_(InitializeExecptionType) {}
public:
	const EInitializeExecptionType& Type() const { return InitializeExecptionType_; }
};

enum class EProjectType
{
	TestProject,
	UnrealEngine,
	VisualStudioCPP
};

class SystemModule : public IBaseModule
{
public:
	explicit SystemModule() noexcept : Super() {}
	virtual ~SystemModule() noexcept {}
public:
	static std::filesystem::path ProjectFolderPath;
public:
	bool bProjectInitialized = false;
	bool bProjectSelected = false;
	EProjectType ProjectType;
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
	virtual void Update() override {}
public:
	void InitializeNewProject(std::filesystem::path projectFolderPath);
	void DeleteProject(std::filesystem::path projectFolderPath);
};