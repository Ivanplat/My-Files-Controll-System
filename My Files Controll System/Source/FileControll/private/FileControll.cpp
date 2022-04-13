#include "FileControll/public/FileControll.h"

#include <iostream>
#include <set>
#include <functional>
#include <map>
#include <thread>


std::set<std::filesystem::path> FileControllComponent::IgnoredFiles = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllComponent::IgnoredDirectories = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllComponent::Directories = std::set<std::filesystem::path>();

bool FileControllComponent::CheckFile(std::filesystem::path FilePath) const
{
	return std::filesystem::is_regular_file(FilePath) || std::filesystem::is_character_file(FilePath) || std::filesystem::is_block_file(FilePath);
}

const bool FileControllComponent::CheckDirectory(std::string DirectoryPath) const
{
	return std::filesystem::is_directory(DirectoryPath);
}

std::string FileControllComponent::GetCurrentDirectory() const
{
	return std::filesystem::current_path().string();
}

std::string FileControllComponent::GetRootDirectory() const
{
	return GetCurrentDirectory();
}

void FileControllComponent::CheckUpdates()
{
	while (true)
	{
		std::cout << "Check" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


bool SpecialCheck(std::set<std::filesystem::path> Dir, std::filesystem::path Path)
{
	auto StringPath = Path.string();
	auto StringPathSize = StringPath.size();

	for (auto& i : Dir)
	{
		auto StringDirPath = i.string();
		auto StringDirSize = StringDirPath.size();
		if (StringPathSize < StringDirSize)
		{
			continue;
		}
		std::string PathCopyString(StringPath.begin(), StringPath.begin() + StringDirSize);
		if (PathCopyString == StringDirPath)
		{
			return true;
		}
	}
	return false;
}

void FileControllComponent::StartupModule()
{
	auto FileControllDirectory = GetRootDirectory();
	std::vector<std::filesystem::path> Files;
	
	for (const auto& i : std::filesystem::recursive_directory_iterator(FileControllDirectory))
	{
		auto path = i.path();
		if (CheckDirectory(path.string()))
		{
			if (!SpecialCheck(IgnoredDirectories, path))
			{
				Directories.insert(path);
			}
		}
	}
	for (const auto& path : Directories)
	{
		for (const auto& i : std::filesystem::directory_iterator(path))
		{
			auto filePath = i.path();
			if (CheckFile(filePath) && IgnoredFiles.find(filePath) == IgnoredFiles.end())
			{
				Files.push_back(filePath);
			}
		}
	}
	for (const auto& i : Files)
	{
		std::cout << i.string() << std::endl;
	}
	Checker = std::thread(&FileControllComponent::CheckUpdates);
	Checker.join();
}
