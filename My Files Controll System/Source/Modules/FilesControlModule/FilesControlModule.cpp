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
	Directories_.clear();
	Files_.clear();
	Directories_        = GC->XML->GetAllFolders();
	Files_              = GC->XML->GetAllFiles();
	IgnoredDirectories_ = GC->XML->GetAllIgnoredFolders();
	IgnoredFiles_       = GC->XML->GetAllIgnoredFiles();
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

void FilesControlModule::MoveFile(std::filesystem::path source, std::filesystem::path destination)
{
	std::filesystem::copy_file(source, destination);
	std::filesystem::remove(source);
}

void FilesControlModule::InitializeProject()
{
	Directories_.clear();
	Files_.clear();
	for (auto& i : std::filesystem::recursive_directory_iterator(GC->System->ProjectFolderPath))
	{
		if (IsDirectory(i) && !GC->XML->FindFolderInIgnored(i))
		{
			Directories_.push_back(i);
		}
	}
	Directories_.push_back(GC->System->ProjectFolderPath);
	for (const auto& directory : Directories_)
	{
		for (auto& i : std::filesystem::directory_iterator(directory))
		{
			if (IsFile(i) && !GC->XML->FindFileInIgnored(i))
			{
				Files_.push_back(i);
			}
		}
	}
	GC->XML->AddCheckOutFolders(Directories_);
	GC->XML->AddCheckOutFiles(Files_);
}

void FilesControlModule::Check
(
	std::vector<std::filesystem::path>* newFiles,
	std::vector<std::filesystem::path>* deletedFiles,
	std::vector<std::filesystem::path>* updatedFiles
)
{
	std::vector<std::filesystem::path> temp_folders_paths;
	std::vector<std::filesystem::path> temp_files_paths;
	for (auto& i : std::filesystem::recursive_directory_iterator(GC->System->ProjectFolderPath))
	{
		if (IsDirectory(i) && !GC->XML->FindFolderInIgnored(i))
		{
			temp_folders_paths.push_back(i);
		}
	}
	temp_folders_paths.push_back(GC->System->ProjectFolderPath);
	for (const auto& folder : temp_folders_paths)
	{
		for (auto& i : std::filesystem::directory_iterator(folder))
		{
			if (IsFile(i) && !GC->XML->FindFileInIgnored(i))
			{
				temp_files_paths.push_back(i);
			}
		}
	}
	std::cout << "\t\tCHECKOUT\n";
	std::cout << "--------------------------------------\n";
	for (const auto& file : temp_files_paths)
	{
		auto it = std::find(Files_.begin(), Files_.end(), file);
		if (it == Files_.end())
		{
			std::cout << "NEW FILE: " << file.string() << "\n";
			if (newFiles) newFiles->push_back(file);
			continue;
		}
		auto last_time_in_xml = GC->XML->GetLastWriteTimeOfFile(file);
		if (last_time_in_xml == FILE_NOT_FOUND_IN_XML)
		{
			std::cerr << "FILE " << file.string() << "IS NOT FOUND IN XML!\nCOULD BE CORRUPTION OF XML FILE!\n";
			continue;
		}
		if (last_time_in_xml != std::filesystem::last_write_time(file).time_since_epoch().count())
		{
			std::cout << "UPDATED FILE: " << file.string() << "\n";
			if(updatedFiles) updatedFiles->push_back(file);
			continue;
		}
	}
	for (const auto& file : Files_)
	{
		auto it = std::find(temp_files_paths.begin(), temp_files_paths.end(), file);
		if (it == temp_files_paths.end())
		{
			std::cout << "FILE DELETED: " << file.string() << "\n";
			if(deletedFiles) deletedFiles->push_back(file);
		}
	}
}

const bool FilesControlModule::IsFile(std::filesystem::path filePath) const
{
	return std::filesystem::is_character_file(filePath) ||
		   std::filesystem::is_regular_file(filePath)   ||
		   std::filesystem::is_block_file(filePath);
}

const bool FilesControlModule::IsDirectory(std::filesystem::path directoryPath) const
{
	return std::filesystem::is_directory(directoryPath);
}