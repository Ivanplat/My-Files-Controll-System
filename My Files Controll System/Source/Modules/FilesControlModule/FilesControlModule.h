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
	std::vector<std::filesystem::path> Files_;
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void Update() override;
public:
	void CreateFile(std::filesystem::path filePath);
	void CreateFolder(std::filesystem::path folderPath);
	void InitializeProject();
public:
	 const bool IsFile(std::filesystem::path filePath) const;
	 const  bool IsDirectory(std::filesystem::path directoryPath) const;
};