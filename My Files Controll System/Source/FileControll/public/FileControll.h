#pragma once
#include "pch.h"
#include "Core/IBaseComponent.h"
#include <set>
#include <thread>
#include <filesystem>
#include <map>

template<class T>
std::set<T> operator -(std::set<T> reference, std::set<T> items_to_remove)
{
	std::set<T> result;
	std::set_difference(reference.begin(), reference.end(), items_to_remove.begin(), items_to_remove.end(), std::inserter(result, result.end()));
	return result;
}

template<class T>
std::set<T> operator +(std::set<T>& reference, std::set<T>& items_to_add)
{
	std::set<T> result;
	result.insert(reference.begin(), reference.end());
	result.insert(items_to_add.begin(), items_to_add.end());
	return result;
}


class FileControllComponent : public virtual IBaseComponent
{
public:
	explicit FileControllComponent(std::string ComponentName) noexcept : Super(ComponentName) {};
public:
	static const bool CheckFile(std::filesystem::path FilePath);
	static const bool CheckDirectory(std::string  DirectoryPath);
	static std::string GetCurrentDirectory();
	static std::string GetRootDirectory();
	static void CheckUpdates(FileControllComponent* self);
	static void CreateFile(std::string FileName);
	void CreateArchiveFromFile(std::filesystem::path Path);
protected:
	static std::set<std::map<std::filesystem::path, std::string>> Files;
	static std::set<std::filesystem::path> Directories;
	static std::set<std::pair<std::filesystem::path, std::string>> IgnoredFiles;
	static std::set<std::filesystem::path> IgnoredDirectories;
	static std::set<std::filesystem::path> AddedDirectories;
	static std::set<std::filesystem::path> RemovedDirectories;
	static std::set<std::filesystem::path> ChangedDirectories;
	std::thread Checker;
private:
	std::set<std::filesystem::path> GetAllDirectories();
	void GetAllFiles();
	const bool CheckEachDirectory(std::string Directory);
	void IsDirectoriesChanged();
	void GetAllIgnored();
public:
	virtual void StartupModule() override;
	virtual void Update() override;
};