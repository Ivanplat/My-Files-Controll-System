#include "pch.h"
#include "Modules/System/public/SystemModule.h"

void SystemModule::StartupModule()
{
}

void SystemModule::Update()
{
}

void SystemModule::InitNewRepository(std::filesystem::path RepositoryPath, std::string RepositoryName)
{
	if (GC->Files->CheckDirectory(RepositoryPath))
	{
		auto FileControllPath = RepositoryPath.string() + "\\FilesControll";
		std::filesystem::create_directory(FileControllPath);

		GC->Files->SetRootPath(RepositoryPath);
		GC->XML->InitializeFiles(RepositoryPath, RepositoryName);
	}
}

FDateTime SystemModule::GetCurrentTime()
{
	return FDateTime();
}
