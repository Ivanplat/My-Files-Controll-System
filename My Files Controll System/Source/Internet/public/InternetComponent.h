#pragma once

#include "Core/IBaseComponent.h"

#include <filesystem>


class InternetComponent : public virtual IBaseComponent
{
public:
	explicit InternetComponent(std::string ComponentName) noexcept : Super(ComponentName) {};
public:
	virtual void StartupModule() override;
	virtual void DownloadFile(std::filesystem::path FilePath, std::string FilePathOnServer);
};