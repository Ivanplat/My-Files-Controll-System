#include "pch.h"

#include "SHA256/SHA256.h"

#include <random>

void SystemModule::StartupModule()
{
	GC->Files->SetRootPath(std::filesystem::current_path());
	GC->Client->StartupModule();
	/*GC->CommandParser->StartupModule();
	GC->XML->StartupModule();
	GC->Files->StartupModule();*/
}

void SystemModule::Update()
{
}

void SystemModule::ShutdownModule()
{
	GC->Client->ShutdownModule();
	for (auto tr : Threads)
	{
		tr.second->detach();
		if (tr.second)
		{
			delete tr.second;
		}
	}
}

void SystemModule::InitNewRepository(std::filesystem::path RepositoryPath, std::string RepositoryName)
{
	if (GC->Files->CheckDirectory(RepositoryPath))
	{
		auto FileControllPath = RepositoryPath.string() + "\\FilesControll";
		if (!GC->Files->CheckDirectory(FileControllPath))
		{

			auto VersionString = CreateVerionHash(CreateRandomString(10));
			std::filesystem::create_directory(FileControllPath);
			std::filesystem::create_directory(FileControllPath+"\\branches");
			std::filesystem::create_directory(FileControllPath + "\\branches\\main");
			std::filesystem::create_directory(FileControllPath + "\\branches\\main\\"+ VersionString);

			GC->Files->SetRootPath(RepositoryPath);
			GC->XML->InitializeFiles(RepositoryPath, RepositoryName, VersionString);
		}
		else
		{
			std::cout << "Already exists" << std::endl;
		}
	}
}

std::string SystemModule::CreateVerionHash(std::string str)
{
	return SHA256::hashString(str);
}

std::string SystemModule::CreateRandomString(size_t StringLengh)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 129);
	std::string result;
	for (size_t i = 0; i < StringLengh; i++)
	{
		result += static_cast<char>(dist(gen));
	}

	return result;
}

FDateTime SystemModule::GetCurrentTime()
{
	return FDateTime();
}
