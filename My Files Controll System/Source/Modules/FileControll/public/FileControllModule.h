#pragma once
#include "pch.h"
#include "Modules/IBaseModule.h"
#include <set>
#include <thread>
#include <filesystem>
#include <map>
#include <stack>

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


class FileControllModule : public virtual IBaseModule
{
public:
	explicit FileControllModule(std::string ComponentName) noexcept : Super(ComponentName) {};
public:
	const bool CheckFile(std::filesystem::path FilePath);
	const bool CheckDirectory(std::filesystem::path  DirectoryPath);
public:
	std::string GetCurrentDirectory();
	std::string HashOfBinaryFile(std::filesystem::path Path);
	std::string BinaryFileToString(std::filesystem::path Path);
	std::string GetFileNameFromPath(std::filesystem::path Path);
public:
	std::filesystem::path GetRootDirectory();
public:
	void CheckUpdates(FileControllModule* self);
	void CreateFile(std::string FilePath);
	void CreateArchiveFromFile(std::filesystem::path Path);
	void SetRootPath(std::filesystem::path NewRootPath);
	void CreateFilesStack(std::vector<std::filesystem::path> Paths);
protected:
	static std::set<std::map<std::filesystem::path, std::string>> Files;
	static std::set<std::filesystem::path> Directories;
	static std::set<std::pair<std::filesystem::path, std::string>> IgnoredFiles;
	static std::set<std::filesystem::path> IgnoredDirectories;
	static std::set<std::filesystem::path> AddedDirectories;
	static std::set<std::filesystem::path> RemovedDirectories;
	static std::set<std::filesystem::path> ChangedDirectories;
	static std::filesystem::path RootPath;
protected:
	std::thread Checker;
private:
	const bool CheckEachDirectory(std::string Directory);
private:
	std::set<std::filesystem::path> GetAllDirectories();
private:
	void GetAllFiles();
	void IsDirectoriesChanged();
	void GetAllIgnored();
public:
	virtual void StartupModule() override;
	virtual void Update() override;
	virtual void ShutdownModule() override;
};