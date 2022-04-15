#include "pch.h"
#include "FileControll/public/FileControll.h"
#include "Core/GarbageCollector.h"

#include "zip.h"


std::set<std::pair<std::filesystem::path, std::string>> FileControllComponent::IgnoredFiles = std::set<std::pair<std::filesystem::path, std::string>>();
std::set<std::filesystem::path> FileControllComponent::IgnoredDirectories = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllComponent::Directories = std::set<std::filesystem::path>(); 
std::set<std::filesystem::path> FileControllComponent::AddedDirectories = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllComponent::RemovedDirectories = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllComponent::ChangedDirectories = std::set<std::filesystem::path>();
std::set<std::map<std::filesystem::path, std::string>> FileControllComponent::Files = std::set<std::map<std::filesystem::path, std::string>>();

const bool FileControllComponent::CheckFile(std::filesystem::path FilePath)
{
	return std::filesystem::is_regular_file(FilePath) || std::filesystem::is_character_file(FilePath) || std::filesystem::is_block_file(FilePath);
}

const bool FileControllComponent::CheckDirectory(std::string DirectoryPath)
{
	return std::filesystem::is_directory(DirectoryPath);
}

std::string FileControllComponent::GetCurrentDirectory()
{
	return std::filesystem::current_path().string();
}

std::string FileControllComponent::GetRootDirectory()
{
	return GetCurrentDirectory();
}

void FileControllComponent::CheckUpdates(FileControllComponent* self)
{
	while (true)
	{
		std::cout << "Check" << std::endl;
		self->IsDirectoriesChanged();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void FileControllComponent::CreateFile(std::string FileName)
{
	std::ofstream outfile(FileName);
	outfile.close();
}

void FileControllComponent::CreateArchiveFromFile(std::filesystem::path Path)
{
	int errors;
	auto z = zip_open(Path.string().c_str(), ZIP_CREATE, &errors);
	auto FileName = GC->XML->GetFileNameFromPath(Path);

	zip_source_t* s;
	const char buf[] = "bufferstring";

	if ((s = zip_source_buffer(z, buf, sizeof(buf), 0)) == NULL ||
		zip_file_add(z, FileName.c_str(), s, ZIP_FL_ENC_UTF_8) < 0) {
		zip_source_free(s);
		printf("error adding file: %s\n", zip_strerror(z));
	}
	std::cout<<zip_close(z)<<std::endl;
	std::cout << errors << std::endl;
	
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

std::set<std::filesystem::path> FileControllComponent::GetAllDirectories()
{
	auto FileControllDirectory = GetRootDirectory();
	std::set<std::filesystem::path> Temp;
	for (const auto& i : std::filesystem::recursive_directory_iterator(FileControllDirectory))
	{
		auto path = i.path();
		if (CheckDirectory(path.string()))
		{
			if (!SpecialCheck(IgnoredDirectories, path))
			{
				Temp.insert(path);
			}
		}
	}
	return Temp;
}

void FileControllComponent::GetAllFiles()
{

}

const bool FileControllComponent::CheckEachDirectory(std::string Directory)
{
	for (const auto& i : std::filesystem::recursive_directory_iterator(Directory))
	{
		auto path = i.path();
		if (CheckDirectory(path.string()))
		{
			if (!SpecialCheck(IgnoredDirectories, path))
			{
				return true;
			}
		}
	}
	return false;
}

void FileControllComponent::IsDirectoriesChanged()
{
	auto NewDirs = GetAllDirectories();
	std::cout << "DIRECTORIES REMOVED" << std::endl;
	std::set<std::filesystem::path> Temp;
	if (Directories - NewDirs != RemovedDirectories)
	{
		for (const auto& i : Directories)
		{
			if (NewDirs.find(i) == NewDirs.end())
			{
				std::cout << i.string() << std::endl;
				Temp = Directories - NewDirs;
				RemovedDirectories.insert(Temp.begin(), Temp.end());
			}
		}
	}
	Temp.clear();
	std::cout << "\n\nDIRECTORIES ADDED" << std::endl;
	if (NewDirs - Directories != AddedDirectories)
	{
		for (const auto& i : NewDirs)
		{
			if (Directories.find(i) == Directories.end())
			{
				Temp = NewDirs - Directories;
				if (!std::filesystem::is_empty(i))
				{
					std::cout << i.string() << std::endl;
					AddedDirectories.insert(Temp.begin(), Temp.end());
				}
			}
		}
	}
	std::cout << "\n\n";

}

void FileControllComponent::GetAllIgnored()
{
	auto ignoredDirectories = GC->XML->GetIgnoredDirectories();
	auto ignoredFiles = GC->XML->GetIgnoredFiles();
	IgnoredDirectories.insert(ignoredDirectories.begin(), ignoredDirectories.end());
	IgnoredFiles.insert(ignoredFiles.begin(), ignoredFiles.end());

	std::cout << std::endl;
	std::cout << std::endl;
	for (const auto& i : IgnoredDirectories)
	{
		std::cout << i.string() << std::endl;
	}
	std::cout << std::endl;
	for (const auto& i : IgnoredFiles)
	{
		std::cout << i.second << std::endl;
	}
}

void FileControllComponent::StartupModule()
{
	Update();
	//Directories = GetAllDirectories();
	//for (const auto& i : Files)
	//{
	//	//std::cout << i.string() << std::endl;
	//}
	/*Checker = std::thread(&FileControllComponent::CheckUpdates, this);
	Checker.join();*/
}

void FileControllComponent::Update()
{
	GetAllIgnored();
}
