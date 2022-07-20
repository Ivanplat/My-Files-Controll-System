#include "pch.h"
#include "FilesControlModule.h"
#include "Modules/SystemModule/SystemModule.h"
#include "Modules/XMLModule/XMLModule.h"
#include <assert.h>

void FilesControlModule::StartupModule()
{
}

void FilesControlModule::ShutdownModule()
{
}

void FilesControlModule::Update()
{
}

void FilesControlModule::CreateFile(std::filesystem::path filePath)
{
	std::ofstream file(filePath, std::ios::out);
	file.write("", 0);
	file.close();
	assert(IsFile(filePath)); //UNABLE TO CREATE A FILE
}

void FilesControlModule::CreateFolder(std::filesystem::path folderPath)
{
	std::filesystem::create_directory(folderPath);
	assert(std::filesystem::is_directory(folderPath)); //UNABLE TO CREATE A FOLDER
}

void FilesControlModule::InitializeProject()
{
	Files_.clear();
	std::vector<std::filesystem::path> temp_directories;
	for (auto& i : std::filesystem::recursive_directory_iterator(GC->System->ProjectFolderPath))
	{
		if (IsDirectory(i) && !GC->XML->FindFolderInIgnored(i))
		{
			temp_directories.push_back(i);
			std::cout << i << std::endl;
		}
	}
	for (const auto& directory : temp_directories)
	{
		for (auto& i : std::filesystem::recursive_directory_iterator(directory))
		{
			if (IsFile(i) && !GC->XML->FindFileInIgnored(i))
			{
				Files_.push_back(i);
			}
		}
	}
	GC->XML->AddCheckOutFiles(Files_);
}

const bool FilesControlModule::IsFile(std::filesystem::path filePath) const
{
	assert(filePath != std::filesystem::path()); //"EMPTY PATH"
	return std::filesystem::is_character_file(filePath) ||
		   std::filesystem::is_regular_file(filePath)   ||
		   std::filesystem::is_block_file(filePath);
}

const bool FilesControlModule::IsDirectory(std::filesystem::path directoryPath) const
{
	assert(directoryPath != std::filesystem::path()); //EMPTY PATH
	return std::filesystem::is_directory(directoryPath);
}
