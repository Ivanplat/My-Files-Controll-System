#pragma once

#include "Modules/IBaseModule.h"

#include <filesystem>


class ClientModule : public virtual IBaseModule
{
public:
	explicit ClientModule(std::string ComponentName) noexcept : Super(ComponentName) {};
public:
	virtual void StartupModule() override;
	virtual void Update() override;
public:
	virtual void DownloadFile(std::filesystem::path FilePath, std::string FilePathOnServer);
};