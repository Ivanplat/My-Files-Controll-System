#pragma once

#include "Modules/IBaseModule.h"
#include <filesystem>

class FilesControlModuleExeption : public ModuleException 
{
public:
	explicit FilesControlModuleExeption(const char* _Message) : ModuleException(_Message) {}
};

class FilesControlModule : public IBaseModule
{
public:
	explicit FilesControlModule() noexcept : Super() {}
private:
	std::vector<std::filesystem::path> Directories_;
	std::vector<std::filesystem::path> Files_;
	std::vector<std::filesystem::path> IgnoredDirectories_;
	std::vector<std::filesystem::path> IgnoredFiles_;
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void Update() override;
public:
	void CreateFile(std::filesystem::path filePath);
	void CreateFolder(std::filesystem::path folderPath);
	void MoveFile(std::filesystem::path source, std::filesystem::path destination);
	void InitializeProject();
	void Check
	(
		std::vector<std::filesystem::path>* newFiles     = nullptr,
		std::vector<std::filesystem::path>* deletedFiles = nullptr,
		std::vector<std::filesystem::path>* updatedFiles = nullptr
	);
public:
	 const bool IsFile(std::filesystem::path filePath) const;
	 const bool IsDirectory(std::filesystem::path directoryPath) const;
};