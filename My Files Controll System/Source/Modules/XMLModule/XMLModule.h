#pragma once

#include "Modules/IBaseModule.h"
#include <filesystem>
#include "tinyxml2.h"

#define FILE_NOT_FOUND_IN_XML   -1
#define FOLDER_NOT_FOUND_IN_XML -1

class XMLModuleExeption : public ModuleException
{
public:
	enum class XMLModuleExeptionType
	{
		XMLFileNotFound,
		CouldNotOpenXMLFile,
		CouldNotSaveXMLFile
	};
private:
	XMLModuleExeptionType XMLExceptionType_;
public:
	explicit XMLModuleExeption(const char* _Message, XMLModuleExeptionType XMLExceptionType) :
															ModuleException(_Message),
															XMLExceptionType_(XMLExceptionType) {}
public:
	const XMLModuleExeptionType& Type() const { return XMLExceptionType_; }
};

class XMLModule : public IBaseModule
{
public:
	std::string IgnoreFileStringPath;
	std::string StatusFileStringPath;
	std::string CheckOutFileStringPath;
	std::string ProjectFolderStringPath;
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
	virtual void Update() override;
public:
	void InitializeNewProject();
	void AddIgnoredFile(std::filesystem::path filePath);
	void AddIgnoredFolder(std::filesystem::path folderPath);
	void AddIgnoredFiles(const std::vector<std::filesystem::path>& filesPaths);
	void AddIgnoredFolders(const std::vector<std::filesystem::path>& foldersPaths);
	void AddCheckOutFiles(const std::vector<std::filesystem::path>& filesPaths);
	void AddCheckOutFolders(const std::vector<std::filesystem::path>& foldersPaths);
	void CreateXMLTask(std::vector<std::filesystem::path> pathsOfFiles);
	void AddNewBranch(std::string branchName);
	void InitializeStatusFile();
	void CreateCommitConfigFile
	(
		std::vector<std::filesystem::path>& newFiles,
		std::vector<std::filesystem::path>& deletedFiles,
		std::vector<std::filesystem::path>& updatedFiles
	);
	void SetBranchCommtiStatus(const std::string& commitFileName);
	std::string GetCurrentBranchName();
	EBranchStatus GetCurrentBranchStatus();
public:
	const bool FindFolderInIgnored(std::filesystem::path folderPath) const;
	const bool FindFileInIgnored(std::filesystem::path filePath) const;
	std::vector<std::filesystem::path> GetAllIgnoredFiles();
	std::vector<std::filesystem::path> GetAllIgnoredFolders();
	std::vector<std::filesystem::path> GetAllFiles();
	std::vector<std::filesystem::path> GetAllFolders();
	int64_t GetLastWriteTimeOfFile(std::filesystem::path filePath);
private:
	void InitializeIgnoreFile();
	void InitializeCheckOutFile();
	void InitializeFilesStringPaths();
	void IncreeseCountOfIntAttribute(tinyxml2::XMLElement* element);
	void InitializeBranchFile(std::string branchName, std::string version, bool bCurrent);
	void UpdateBranchFile
	(
		std::string branchName,
		std::string version,
		std::vector<std::filesystem::path> files,
		std::vector<std::filesystem::path> folders
	);
};