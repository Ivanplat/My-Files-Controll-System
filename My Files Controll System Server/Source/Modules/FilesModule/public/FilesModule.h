#pragma once

#include <filesystem>

#include "Modules/IBaseModule.h"


class FilesModule : public virtual IBaseModule
{
public:
	explicit FilesModule(std::string ModuleName) noexcept : Super(ModuleName) {};
public:
	virtual void StartupModule() override;
	virtual void Update() override;
	virtual void ShutdownModule() override;
public:
	virtual void CreateFile(std::filesystem::path Path);
	virtual void DeleteFile(std::filesystem::path Path);
	virtual void CreateZipFromFile(std::filesystem::path Path);
public:
	const bool CheckFile(std::filesystem::path Path) const;
	const bool CheckDirectory(std::filesystem::path Path) const;
};