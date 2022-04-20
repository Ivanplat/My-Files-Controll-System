#pragma once


#include "Modules/IBaseModule.h"
#include "tinyxml2.h"

#include <memory>
#include <filesystem>
#include <map>


class XMLModule : public virtual IBaseModule
{
public:
	explicit XMLModule(std::string ComponentName) noexcept : Super(ComponentName) {};
public:
	
public:
	virtual void StartupModule() override;
	virtual void Update() override;
private:
	virtual void InitIgnoreFile();
	virtual void InitCheckoutFile();
	virtual void InitStatusFile(std::string RepositoryName = std::string());
	virtual void InitBranchVersionInfoFile(std::string BranchName, std::string VersionString);
public:
	virtual void AddFileToIgnore(std::filesystem::path Path);
	virtual void AddDirectoryToIgnore(std::filesystem::path Path);
	virtual void RemoveFileFromIgnore(std::filesystem::path Path);
	virtual void RemoveDirectoryFromIgnore(std::filesystem::path Path);
	void SaveIgnoreDocument();
	void LoadIgnoreDocument();
	void InitializeFiles(std::filesystem::path RepositoryPath, std::string RepositoryName, std::string VersionString);
	std::vector<std::filesystem::path> GetIgnoredDirectories();
	std::vector<std::pair<std::filesystem::path, std::string>> GetIgnoredFiles();
protected:
	std::unique_ptr<tinyxml2::XMLDocument> IgnoreDocument;
};