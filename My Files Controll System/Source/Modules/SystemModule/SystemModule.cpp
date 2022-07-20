#include "pch.h"
#include "SystemModule.h"
#include "Modules/FilesControlModule/FilesControlModule.h"
#include "Modules/XMLModule/XMLModule.h"
#include <assert.h>

std::filesystem::path SystemModule::ProjectFolderPath = std::filesystem::path();

void SystemModule::InitializeNewProject(std::filesystem::path projectFolderPath)
{
	assert(projectFolderPath != std::filesystem::path("")); //EMPTY PATH
	std::filesystem::path temp_FolderPath       = projectFolderPath.string() + "\\.FilesControl";
	std::filesystem::path temp_CheckOutFilePath = temp_FolderPath.string() + "\\checkout.xml";
	std::filesystem::path temp_StatusFilePath   = temp_FolderPath.string() + "\\status.xml";
	std::filesystem::path temp_IgnoreFilePath   = temp_FolderPath.string() + "\\ignore.xml";
	if (!GC->FilesControl->IsDirectory(temp_FolderPath))
	{
		GC->FilesControl->CreateFolder(temp_FolderPath);
		GC->FilesControl->CreateFile(temp_CheckOutFilePath);
		GC->FilesControl->CreateFile(temp_StatusFilePath);
		GC->FilesControl->CreateFile(temp_IgnoreFilePath);
		bProjectInitialized = true;
		bProjectSelected    = true;
		ProjectFolderPath   = projectFolderPath;
		GC->XML->InitializeNewProject();
		GC->FilesControl->InitializeProject();
	}
}

void SystemModule::DeleteProject(std::filesystem::path projectFolderPath)
{
	assert(projectFolderPath != std::filesystem::path("")); //EMPTY PATH
	std::filesystem::path temp_FolderPath = projectFolderPath.string() + "\\.FilesControl";
	if (GC->FilesControl->IsDirectory(temp_FolderPath))
	{
		std::filesystem::remove_all(temp_FolderPath);
		bProjectInitialized = false;
		bProjectSelected    = false;
		ProjectFolderPath   = std::filesystem::path();
	}
}
