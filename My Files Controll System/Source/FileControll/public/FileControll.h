#pragma once

#include "Core/IBaseComponent.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <thread>

class FileControllComponent : public virtual IBaseComponent
{
public:
	explicit FileControllComponent(std::string ComponentName) noexcept : Super(ComponentName) {};
public:
	bool CheckFile(std::filesystem::path FilePath) const;
	const bool CheckDirectory(std::string  DirectoryPath) const;
	std::string GetCurrentDirectory() const;
	std::string GetRootDirectory() const;
	static void CheckUpdates();
protected:
	static std::set<std::map<std::filesystem::path, std::string>> Files;
	static std::set<std::filesystem::path> Directories;
	static std::set<std::filesystem::path> IgnoredFiles;
	static std::set<std::filesystem::path> IgnoredDirectories;
	std::thread Checker;
public:
	virtual void StartupModule() override;
};