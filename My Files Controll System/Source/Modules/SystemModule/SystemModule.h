#pragma once

#include "Modules/IBaseModule.h"
#include <filesystem>

#define FOLDER_DOES_NOT_EXIST_VALUE   -1
#define PROJECT_IS_ALREADY_INITIALIZED 0
#define COULD_INITIALIZE_NEW_PROJECT   1


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

class SystemModule : public IBaseModule
{
public:
	typedef int INITIALIZE_ABILITY;
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
	void SelectProject(std::filesystem::path projectFolderPath);
	void AddNewBranch(std::string branchName);
	void Commit();
	void Push();
public:
	const INITIALIZE_ABILITY CouldInitilializeProject(std::filesystem::path directoryPath) const;
private:
	const bool IsProjectCorrupted(std::filesystem::path projectFolderPath) const;
	const bool CouldProjectBeDeleted(std::filesystem::path projectFolderPath) const;
	const bool CouldCommitCurrentBranch() const;
};