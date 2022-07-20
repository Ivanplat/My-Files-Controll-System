#pragma once

#include "Modules/IBaseModule.h"
#include <filesystem>
#include "tinyxml2.h"

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
	virtual void Update() override {}
public:
	void InitializeNewProject();
	void AddIgnoredFile(std::filesystem::path filePath);
	void AddIgnoredFolder(std::filesystem::path folderPath);
	void AddIgnoredFiles(const std::vector<std::filesystem::path>& filesPaths);
	void AddIgnoredFolders(const std::vector<std::filesystem::path>& foldersPaths);
	void AddCheckOutFiles(const std::vector<std::filesystem::path>& filesPaths);
	void CreateXMLTask(std::vector<std::filesystem::path> pathsOfFiles);
public:
	const bool FindFolderInIgnored(std::filesystem::path folderPath) const;
	const bool FindFileInIgnored(std::filesystem::path filePath) const;
private:
	void InitializeIgnoreFile();
	void InitializeStatusFile();
	void InitializeCheckOutFile();
	void InitializeFilesStringPaths();
	void IncreeseCountOfIntAttribute(tinyxml2::XMLElement* element);
};