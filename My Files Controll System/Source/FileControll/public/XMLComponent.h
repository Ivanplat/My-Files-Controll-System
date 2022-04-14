#pragma once


#include "Core/IBaseComponent.h"
#include "tinyxml2.h"

#include <memory>
#include <filesystem>


class XMLComponent : public virtual IBaseComponent
{
public:
	explicit XMLComponent(std::string ComponentName) noexcept : Super(ComponentName) {};
public:
	
public:
	virtual void StartupModule() override;
private:
	virtual void InitIgnoreFile();
	virtual void InitCheckoutFile();
	virtual void InitStatusFile();
public:
	virtual void AddFileToIgnore(std::filesystem::path Path, std::string Hash);
	virtual void AddDirectoryToIgnore(std::filesystem::path Path);
	virtual void RemoveFileFromIgnore(std::filesystem::path Path);
	virtual void RemoveDirectoryFromIgnore(std::filesystem::path Path);
	void SaveIgnoreDocument();
	void LoadIgnoreDocument();
	std::string GetFileNameFromPath(std::filesystem::path Path);
protected:
	std::unique_ptr<tinyxml2::XMLDocument> IgnoreDocument;
};