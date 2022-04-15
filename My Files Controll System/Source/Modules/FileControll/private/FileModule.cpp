#include "pch.h"
#include "Modules/FileControll/public/FileControllModule.h"
#include "Core/GarbageCollector.h"

#include "zip.h"


std::set<std::pair<std::filesystem::path, std::string>> FileControllModule::IgnoredFiles = std::set<std::pair<std::filesystem::path, std::string>>();
std::set<std::filesystem::path> FileControllModule::IgnoredDirectories = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllModule::Directories = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllModule::AddedDirectories = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllModule::RemovedDirectories = std::set<std::filesystem::path>();
std::set<std::filesystem::path> FileControllModule::ChangedDirectories = std::set<std::filesystem::path>();
std::set<std::map<std::filesystem::path, std::string>> FileControllModule::Files = std::set<std::map<std::filesystem::path, std::string>>();
std::filesystem::path FileControllModule::RootPath = std::filesystem::path();

const bool FileControllModule::CheckFile(std::filesystem::path FilePath)
{
	return std::filesystem::is_regular_file(FilePath) || std::filesystem::is_character_file(FilePath) || std::filesystem::is_block_file(FilePath);
}

const bool FileControllModule::CheckDirectory(std::filesystem::path DirectoryPath)
{
	return std::filesystem::is_directory(DirectoryPath);
}

std::string FileControllModule::GetCurrentDirectory()
{
	return std::filesystem::current_path().string();
}

std::filesystem::path FileControllModule::GetRootDirectory()
{
	return RootPath;
}

void FileControllModule::CheckUpdates(FileControllModule* self)
{
	while (true)
	{
		std::cout << "Check" << std::endl;
		self->IsDirectoriesChanged();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void FileControllModule::CreateFile(std::string FilePath)
{
	std::cout << FilePath << " ";
	std::ofstream outfile(FilePath);
	std::cout << outfile.is_open()<<std::endl;
	outfile.close();
}

void FileControllModule::CreateArchiveFromFile(std::filesystem::path Path)
{
	int errors;
	auto z = zip_open(Path.string().c_str(), ZIP_CREATE, &errors);
	auto FileName = GetFileNameFromPath(Path);

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

std::string FileControllModule::GetFileNameFromPath(std::filesystem::path Path)
{
	GC->Log->PrintToLog("FileControllComponent::GetFileNameFromPath()");
	std::string buf;
	auto str = Path.string();
	for (int i = str.size() - 1; i >= 0; i--)
	{
		if (str[i] != '\\')
		{
			buf += str[i];
		}
		else
		{
			break;
		}
	}
	std::reverse(buf.begin(), buf.end());
	return buf;
}

void FileControllModule::SetRootPath(std::filesystem::path NewRootPath)
{
	RootPath = NewRootPath;
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

std::set<std::filesystem::path> FileControllModule::GetAllDirectories()
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

void FileControllModule::GetAllFiles()
{

}

const bool FileControllModule::CheckEachDirectory(std::string Directory)
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

void FileControllModule::IsDirectoriesChanged()
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

void FileControllModule::GetAllIgnored()
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

void FileControllModule::StartupModule()
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

void FileControllModule::Update()
{
	GetAllIgnored();
}
