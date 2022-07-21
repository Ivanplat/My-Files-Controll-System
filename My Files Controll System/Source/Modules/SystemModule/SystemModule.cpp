#include "pch.h"
#include "SystemModule.h"
#include "Modules/FilesControlModule/FilesControlModule.h"
#include "Modules/XMLModule/XMLModule.h"
#include <assert.h>
#include <algorithm>


std::filesystem::path SystemModule::ProjectFolderPath = std::filesystem::path();

void SystemModule::InitializeNewProject(std::filesystem::path projectFolderPath)
{
	assert(projectFolderPath != std::filesystem::path("")); //EMPTY PATH
	std::filesystem::path temp_FolderPath         = projectFolderPath.string() + "\\.FilesControl";
	std::filesystem::path temp_BranchesFolderPath = temp_FolderPath.string()   + "\\branches";
	std::filesystem::path temp_CheckOutFilePath   = temp_FolderPath.string()   + "\\checkout.xml";
	std::filesystem::path temp_StatusFilePath     = temp_FolderPath.string()   + "\\status.xml";
	std::filesystem::path temp_IgnoreFilePath     = temp_FolderPath.string()   + "\\ignore.xml";
	if (!GC->FilesControl->IsDirectory(temp_FolderPath))
	{
		GC->FilesControl->CreateFolder(temp_FolderPath);
		GC->FilesControl->CreateFolder(temp_BranchesFolderPath);
		GC->FilesControl->CreateFile(temp_CheckOutFilePath);
		GC->FilesControl->CreateFile(temp_StatusFilePath);
		GC->FilesControl->CreateFile(temp_IgnoreFilePath);
		bProjectInitialized = true;
		bProjectSelected    = true;
		ProjectFolderPath   = projectFolderPath;
		GC->XML->InitializeNewProject();
		GC->XML->InitializeStatusFile();
		Commit();
		Push();
		GC->FilesControl->InitializeProject();
	}
}

void SystemModule::DeleteProject(std::filesystem::path projectFolderPath)
{
	assert(projectFolderPath != std::filesystem::path("")); //EMPTY PATH
	if (!CouldProjectBeDeleted(projectFolderPath))
	{
		throw SystemModuleException("NOT A PROJECT!");
	}
	std::filesystem::path temp_FolderPath = projectFolderPath.string() + "\\.FilesControl";
	if (GC->FilesControl->IsDirectory(temp_FolderPath))
	{
		std::filesystem::remove_all(temp_FolderPath);
		bProjectInitialized = false;
		bProjectSelected    = false;
		ProjectFolderPath   = std::filesystem::path();
	}
}

void SystemModule::SelectProject(std::filesystem::path projectFolderPath)
{
	if (IsProjectCorrupted(projectFolderPath))
	{
		throw SystemModuleException("PROJECT IS CORRUPTED, THEREFORE, CAN NOT BE SELECTED!");
	}
	bProjectInitialized = true;
	bProjectSelected = true;
	ProjectFolderPath = projectFolderPath;
	GC->XML->Update();
	GC->FilesControl->Update();
}

void SystemModule::AddNewBranch(std::string branchName)
{
	std::filesystem::path temp_BranchesFolderPath  = ProjectFolderPath.string()       + "\\.FilesControl\\branches";
	std::filesystem::path temp_NewBranchFolderPath = temp_BranchesFolderPath.string() + "\\" + branchName;
	GC->FilesControl->CreateFolder(temp_NewBranchFolderPath);
	GC->XML->AddNewBranch(branchName);

}

void SystemModule::Commit()
{
	if (!bProjectInitialized && !bProjectSelected)
	{
		throw SystemModuleException("PROJECT IS NOT INITIALIZED OR SELECTED YET!");
	}
	if (GC->XML->GetCurrentBranchStatus() == EBranchStatus::Commiting)
	{
		throw SystemModuleException("BRANCH IS ALREADY COMMITED!");
	}
	if (GC->XML->GetCurrentBranchStatus() == EBranchStatus::Pulling)
	{
		throw SystemModuleException("BRANCH IS PULLING AT THIS MOMENT!");
	}
	if (GC->XML->GetCurrentBranchStatus() == EBranchStatus::Pushing)
	{
		throw SystemModuleException("BRANCH IS PUSHING AT THIS MOMENT!");
	}
	if (GC->XML->GetCurrentBranchStatus() == EBranchStatus::Merging)
	{
		throw SystemModuleException("BRANCH IS MERGING AT THIS MOMENT!");
	}
	if (GC->XML->GetCurrentBranchStatus() == EBranchStatus::Changing)
	{
		throw SystemModuleException("BRANCH IS CHANGING AT THIS MOMENT!");
	}
	std::vector<std::filesystem::path> newFiles;
	std::vector<std::filesystem::path> deletedFiles;
	std::vector<std::filesystem::path> updatedFiles;
	GC->FilesControl->Check(&newFiles, &deletedFiles, &updatedFiles);
	GC->XML->CreateCommitConfigFile(newFiles, deletedFiles, updatedFiles);
}

void SystemModule::Push()
{
}

const SystemModule::INITIALIZE_ABILITY SystemModule::CouldInitilializeProject(std::filesystem::path directoryPath) const
{
	if (!GC->FilesControl->IsDirectory(directoryPath))
	{
		return FOLDER_DOES_NOT_EXIST_VALUE;
	}
	std::filesystem::path temp_FolderPath = directoryPath.string() + "\\.FilesControl";
	if (GC->FilesControl->IsDirectory(temp_FolderPath))
	{
		return PROJECT_IS_ALREADY_INITIALIZED;
	}
	return COULD_INITIALIZE_NEW_PROJECT;
}

const bool SystemModule::IsProjectCorrupted(std::filesystem::path projectFolderPath) const
{
	std::filesystem::path temp_FolderPath         = projectFolderPath.string() + "\\.FilesControl";
	std::filesystem::path temp_BranchesFolderPath = temp_FolderPath.string()   + "\\branches\\main";
	std::filesystem::path temp_CheckOutFilePath   = temp_FolderPath.string()   + "\\checkout.xml";
	std::filesystem::path temp_StatusFilePath     = temp_FolderPath.string()   + "\\status.xml";
	std::filesystem::path temp_IgnoreFilePath     = temp_FolderPath.string()   + "\\ignore.xml";
	if
		(
			!GC->FilesControl->IsDirectory(temp_FolderPath) ||
			!GC->FilesControl->IsDirectory(temp_BranchesFolderPath) ||
			!GC->FilesControl->IsFile(temp_CheckOutFilePath) ||
			!GC->FilesControl->IsFile(temp_StatusFilePath) ||
			!GC->FilesControl->IsFile(temp_IgnoreFilePath)
			)
	{
		return true;
	}
	auto it = std::filesystem::directory_iterator(temp_BranchesFolderPath);
	int filesCount = 0;
	for (auto& entry : it)
	{
		++filesCount;
	}
	if (filesCount == 0) return true;

	return false;
}

const bool SystemModule::CouldProjectBeDeleted(std::filesystem::path projectFolderPath) const
{
	std::filesystem::path temp_FolderPath = projectFolderPath.string() + "\\.FilesControl";
	return GC->FilesControl->IsDirectory(temp_FolderPath);
}

const bool SystemModule::CouldCommitCurrentBranch() const
{
	return GC->XML->GetCurrentBranchStatus() == EBranchStatus::OK;
}
