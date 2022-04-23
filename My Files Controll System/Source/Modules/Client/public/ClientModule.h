#pragma once

#include "Modules/IBaseModule.h"

#include <filesystem>
#include <stack>
#include <basetsd.h>


class ClientModule : public virtual IBaseModule
{
public:
	explicit ClientModule(std::string ComponentName) noexcept : Super(ComponentName) {};
public:
	virtual void StartupModule() override;
	virtual void Update() override;
	virtual void ShutdownModule() override;
public:
	virtual void DownloadFile(std::filesystem::path FilePath, std::string FilePathOnServer);
	virtual void SendFiles(UINT_PTR* sock);
public:
	std::stack<std::string> FilesStack;
};