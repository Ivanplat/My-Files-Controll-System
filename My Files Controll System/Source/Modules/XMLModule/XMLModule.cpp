#include "pch.h"
#include "XMLModule.h"
#include "Modules/SystemModule/SystemModule.h"
#include "Modules/FilesControlModule/FilesControlModule.h"
#include "SHA256/SHA256.h"
#include <time.h>


void XMLModule::Update()
{
	if (!GC->System->bProjectInitialized && !GC->System->bProjectSelected)
	{
		throw ProjectInitializeExeption("PROJECT IS NOT INITIALIZED OR NOT SELECTED YET!",
			ProjectInitializeExeption::EInitializeExecptionType::NotInitializedOrSelected);
	}
	InitializeFilesStringPaths();
}

void XMLModule::InitializeNewProject()
{
	if (!GC->System->bProjectInitialized && !GC->System->bProjectSelected)
	{
		throw ProjectInitializeExeption("PROJECT IS NOT INITIALIZED OR NOT SELECTED YET!",
			  ProjectInitializeExeption::EInitializeExecptionType::NotInitializedOrSelected);
	}
	InitializeFilesStringPaths();
	InitializeIgnoreFile();
	InitializeCheckOutFile();
}

void XMLModule::AddIgnoredFile(std::filesystem::path filePath)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("files");
	auto fileElement = doc.NewElement("file");
	fileElement->SetText(filePath.string().c_str());
	filesElement->InsertEndChild(fileElement);
	IncreeseCountOfIntAttribute(filesElement);
	if (doc.SaveFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::AddIgnoredFolder(std::filesystem::path folderPath)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("folders");
	auto fileElement = doc.NewElement("folder");
	fileElement->SetText(folderPath.string().c_str());
	filesElement->InsertEndChild(fileElement);
	IncreeseCountOfIntAttribute(filesElement);
	if (doc.SaveFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::AddIgnoredFiles(const std::vector<std::filesystem::path>& filesPaths)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("files");
	for (const auto& filePath : filesPaths)
	{
		auto fileElement = doc.NewElement("file");
		fileElement->SetText(filePath.string().c_str());
		filesElement->InsertEndChild(fileElement);
		IncreeseCountOfIntAttribute(filesElement);
	}
	if (doc.SaveFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::AddIgnoredFolders(const std::vector<std::filesystem::path>& foldersPaths)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("folders");
	for (const auto& folderPath : foldersPaths)
	{
		auto fileElement = doc.NewElement("folder");
		fileElement->SetText(folderPath.string().c_str());
		filesElement->InsertEndChild(fileElement);
		IncreeseCountOfIntAttribute(filesElement);
	}
	if (doc.SaveFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::AddCheckOutFiles(const std::vector<std::filesystem::path>& filesPaths)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(CheckOutFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE CHECKOUT FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("files");
	for (const auto& filePath : filesPaths)
	{
		auto fileElement = doc.NewElement("file");
		fileElement->SetText(filePath.string().c_str());
		auto lastWriteTime = std::filesystem::last_write_time(filePath);
		fileElement->SetAttribute("change_time", lastWriteTime.time_since_epoch().count());
		filesElement->InsertEndChild(fileElement);
		IncreeseCountOfIntAttribute(filesElement);
	}
	if (doc.SaveFile(CheckOutFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE CHECKOUT FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::AddCheckOutFolders(const std::vector<std::filesystem::path>& foldersPaths)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(CheckOutFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE CHECKOUT FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto foldersElement = head->FirstChildElement("folders");
	for (const auto& folderPath : foldersPaths)
	{
		auto fileElement = doc.NewElement("folder");
		fileElement->SetText(folderPath.string().c_str());
		foldersElement->InsertEndChild(fileElement);
		IncreeseCountOfIntAttribute(foldersElement);
	}
	if (doc.SaveFile(CheckOutFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE CHECKOUT FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::CreateXMLTask(std::vector<std::filesystem::path> pathsOfFiles)
{
	tinyxml2::XMLDocument doc;
	auto head = doc.NewElement("task");
	auto filesElement = doc.NewElement("files");
	for (const auto& filePath : pathsOfFiles)
	{
		auto fileElement = doc.NewElement("file");
		fileElement->SetText(filePath.c_str());
		filesElement->InsertEndChild(fileElement);
	}
	head->InsertEndChild(filesElement);
	doc.InsertEndChild(head);
	if (doc.SaveFile((ProjectFolderStringPath + "\\task.xml").c_str()) !=
		tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE TASK FILE!", 
			  XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::AddNewBranch(std::string branchName)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(StatusFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE STATUS FILE!",
			  XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto branchesElement = head->FirstChildElement("branches");
	auto newBranchElement = doc.NewElement("branch");
	auto newBranchNameElement = doc.NewElement("name");
	newBranchNameElement->SetText(branchName.c_str());
	auto newBranchStatusElement = doc.NewElement("status");
	newBranchStatusElement->SetText("OK");
	auto newBranchCurrentVersionElement = doc.NewElement("current_version");
	auto versionString = SHA256::hashString
	(
		std::to_string
		(
			std::filesystem::last_write_time(StatusFileStringPath).time_since_epoch().count()
		)
	);
	newBranchCurrentVersionElement->SetText(versionString.c_str());
	newBranchElement->InsertEndChild(newBranchNameElement);
	newBranchElement->InsertEndChild(newBranchCurrentVersionElement);
	newBranchElement->InsertEndChild(newBranchStatusElement);
	branchesElement->InsertEndChild(newBranchElement);
	head->InsertEndChild(branchesElement);
	auto newBranchFolderPath = ProjectFolderStringPath + "\\branches\\" + branchName;
	auto newBranchCurrentVersionPath = newBranchFolderPath + "\\" + versionString;
	auto pathToBranchFile = newBranchCurrentVersionPath + "\\branch_status.xml";
	GC->FilesControl->CreateFolder(newBranchFolderPath);
	GC->FilesControl->CreateFolder(newBranchCurrentVersionPath);
	GC->FilesControl->CreateFile(pathToBranchFile);
	if (doc.SaveFile(StatusFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE STATUS FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
	InitializeBranchFile(branchName, versionString, false);
}

const bool XMLModule::FindFolderInIgnored(std::filesystem::path folderPath) const
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto foldersElement = head->FirstChildElement("folders");
	auto folderElement = foldersElement->FirstChildElement();
	while (folderElement)
	{
		auto pathString = std::string(folderElement->GetText());
		if (folderPath.string().size() < pathString.size())
		{
			return false;
		}
		if (folderPath.string() == pathString)
		{
			return true;
		}
		std::string cpy = pathString;
		folderPath.string().copy(cpy.data(), pathString.size(), 0U);
		if (cpy == pathString)
		{
			return true;
		}
		folderElement = folderElement->NextSiblingElement();
	}
	return false;
}

const bool XMLModule::FindFileInIgnored(std::filesystem::path filePath) const
{
	if (FindFolderInIgnored(filePath)) return true;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("files");
	auto fileElement = filesElement->FirstChildElement();
	while (fileElement)
	{
		if (filePath.string() == fileElement->GetText())
		{
			return true;
		}
		fileElement = fileElement->NextSiblingElement();
	}
	return false;
}

std::vector<std::filesystem::path> XMLModule::GetAllIgnoredFiles()
{
	std::vector<std::filesystem::path> result;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("files");
	auto fileElement = filesElement->FirstChildElement();
	while (fileElement)
	{
		auto path_c_str = fileElement->GetText();
		result.push_back(std::string(path_c_str));
		fileElement = fileElement->NextSiblingElement();
	}
	return result;
}

std::vector<std::filesystem::path> XMLModule::GetAllIgnoredFolders()
{
	std::vector<std::filesystem::path> result;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto foldersElement = head->FirstChildElement("folders");
	auto folderElement = foldersElement->FirstChildElement();
	while (folderElement)
	{
		auto path_c_str = folderElement->GetText();
		result.push_back(std::string(path_c_str));
		folderElement = folderElement->NextSiblingElement();
	}
	return result;
}

std::vector<std::filesystem::path> XMLModule::GetAllFiles()
{
	std::vector<std::filesystem::path> result;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(CheckOutFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE CHECKOUT FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("files");
	auto fileElement = filesElement->FirstChildElement();
	while (fileElement)
	{
		auto path_c_str = fileElement->GetText();
		result.push_back(std::string(path_c_str));
		fileElement = fileElement->NextSiblingElement();
	}
	return result;
}

std::vector<std::filesystem::path> XMLModule::GetAllFolders()
{
	std::vector<std::filesystem::path> result;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(CheckOutFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE CHECKOUT FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto foldersElement = head->FirstChildElement("folders");
	auto folderElement = foldersElement->FirstChildElement();
	while (folderElement)
	{
		auto path_c_str = folderElement->GetText();
		result.push_back(std::string(path_c_str));
		folderElement = folderElement->NextSiblingElement();
	}
	return result;
}

int64_t XMLModule::GetLastWriteTimeOfFile(std::filesystem::path filePath)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(CheckOutFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE CHECKOUT FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("files");
	auto fileElement = filesElement->FirstChildElement();
	while (fileElement)
	{
		if (filePath.string() == fileElement->GetText())
		{
			return fileElement->Int64Attribute("change_time");
		}
		fileElement = fileElement->NextSiblingElement();
	}
	return FILE_NOT_FOUND_IN_XML;
}

void XMLModule::InitializeIgnoreFile()
{
	tinyxml2::XMLDocument doc;
	auto head = doc.NewElement("ignore");
	auto filesElement = doc.NewElement("files");
	filesElement->SetAttribute("count", 0);
	auto foldersElement = doc.NewElement("folders");
	foldersElement->SetAttribute("count", 1);
	auto alwaysIgnoredFolderElement = doc.NewElement("folder");
	alwaysIgnoredFolderElement->SetText(ProjectFolderStringPath.c_str());
	foldersElement->InsertEndChild(alwaysIgnoredFolderElement);
	head->InsertEndChild(filesElement);
	head->InsertEndChild(foldersElement);
	doc.InsertEndChild(head);
	if(doc.SaveFile(IgnoreFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE IGNORE FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::InitializeStatusFile()
{
	tinyxml2::XMLDocument doc;
	auto head = doc.NewElement("status");
	head->SetAttribute("version", "1.0");
	auto branchesElement = doc.NewElement("branches");
	branchesElement->SetAttribute("count", 1);
	auto mainBranchElement = doc.NewElement("branch");
	auto mainBranchNameElement = doc.NewElement("name");
	mainBranchNameElement->SetText("main");
	auto mainBranchStatusElement = doc.NewElement("status");
	mainBranchStatusElement->SetText("OK");
	auto mainBranchCurrentVersionElement = doc.NewElement("current_version");
	auto isMainCurrentBranchElement = doc.NewElement("is_current");
	auto versionString = SHA256::hashString
	(
		std::to_string
		(
			std::filesystem::last_write_time(StatusFileStringPath).time_since_epoch().count()
		)
	);
	isMainCurrentBranchElement->SetText(true);
	mainBranchCurrentVersionElement->SetText(versionString.c_str());
	mainBranchElement->InsertEndChild(mainBranchNameElement);
	mainBranchElement->InsertEndChild(mainBranchCurrentVersionElement);
	mainBranchElement->InsertEndChild(mainBranchStatusElement);
	branchesElement->SetAttribute("current_branch", "main");
	branchesElement->InsertEndChild(mainBranchElement);
	head->InsertEndChild(branchesElement);
	doc.InsertEndChild(head);
	if (doc.SaveFile(StatusFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE STATUS FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
	auto mainBranchFolderPath         = ProjectFolderStringPath + "\\branches\\main";
	auto mainBranchCurrentVersionPath = mainBranchFolderPath    + "\\" + versionString;
	GC->FilesControl->CreateFolder(mainBranchFolderPath);
	GC->FilesControl->CreateFolder(mainBranchCurrentVersionPath);
	InitializeBranchFile("main", versionString, true);
}

void XMLModule::CreateCommitConfigFile
(
	std::vector<std::filesystem::path>& newFiles,
	std::vector<std::filesystem::path>& deletedFiles,
	std::vector<std::filesystem::path>& updatedFiles
)
{
	[[maybe_unused]] tm t;
	[[maybe_unused]] time_t long_time;
	_time64(&long_time);
	localtime_s(&t, &long_time);
	std::string stringTime = std::to_string(long_time);
	std::string commitConfigFileName = "commit_" + stringTime + ".xml";
	std::string stringConfigFilePath = ProjectFolderStringPath + "\\" + commitConfigFileName;
	GC->FilesControl->CreateFile(stringConfigFilePath);
	tinyxml2::XMLDocument doc;
	auto head				 = doc.NewElement("commit");
	auto filesElement		 = doc.NewElement("files");
	auto newFilesElement	 = doc.NewElement("new_files");
	auto deletedFilesElement = doc.NewElement("deleted_files");
	auto updatedFilesElement = doc.NewElement("updated_files");
	std::function<void
	(
		std::vector<std::filesystem::path>&,
		tinyxml2::XMLDocument* doc,
		tinyxml2::XMLElement*,
		bool
	)> insertFileInfoInCategory = 
		[]
	(
		std::vector<std::filesystem::path>& files,
		tinyxml2::XMLDocument* doc,
		tinyxml2::XMLElement* parent,
		bool bNeedLastTime
	) 
	{
		assert(doc);
		assert(parent);
		for (const auto& file : files)
		{
			auto fileElement = doc->NewElement("file");
			fileElement->SetText(file.string().c_str());
			if (bNeedLastTime)
			{
				auto lastWriteTime = std::filesystem::last_write_time(file);
				fileElement->SetAttribute("change_time", lastWriteTime.time_since_epoch().count());
			}
			parent->InsertEndChild(fileElement);
		}
	};
	insertFileInfoInCategory(newFiles,     &doc, newFilesElement,     true );
	insertFileInfoInCategory(deletedFiles, &doc, deletedFilesElement, false);
	insertFileInfoInCategory(updatedFiles, &doc, updatedFilesElement, true );
	filesElement->InsertEndChild(newFilesElement);
	filesElement->InsertEndChild(deletedFilesElement);
	filesElement->InsertEndChild(updatedFilesElement);
	head->InsertEndChild(filesElement);
	doc.InsertEndChild(head);
	if (doc.SaveFile(stringConfigFilePath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE COMMIT CONFIG FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
	SetBranchCommtiStatus(commitConfigFileName.c_str());
}

void XMLModule::SetBranchCommtiStatus(const std::string& commitFileName)
{
	auto currentBranchName = GetCurrentBranchName();
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(StatusFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE STATUS FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto branchesElement = head->FirstChildElement("branches");
	auto branchElement = branchesElement->FirstChildElement();
	while (branchElement)
	{
		auto nameElement = branchElement->FirstChildElement("name");
		if (nameElement->GetText() == currentBranchName)
		{
			break;
		}
		branchElement = branchElement->NextSiblingElement();
	}
	auto branchStatusElement = branchElement->FirstChildElement("status");
	branchStatusElement->SetText("commiting");
	auto commitConfigNameElement = doc.NewElement("commit_config_name");
	commitConfigNameElement->SetText(commitFileName.c_str());
	branchElement->InsertEndChild(commitConfigNameElement);
	if (doc.SaveFile(StatusFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE STATUS FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

std::string XMLModule::GetCurrentBranchName()
{
	[[maybe_unused]] const char* result;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(StatusFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE STATUS FILE!",
			  XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto branchesElement = head->FirstChildElement("branches");
	branchesElement->QueryStringAttribute("current_branch", &result);
	return std::string(result);
}

EBranchStatus XMLModule::GetCurrentBranchStatus()
{
	auto currentBranchName = GetCurrentBranchName();
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(StatusFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT OPEN THE STATUS FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto branchesElement = head->FirstChildElement("branches");
	auto branchElement = branchesElement->FirstChildElement();
	while (branchElement)
	{
		auto nameElement = branchElement->FirstChildElement("name");
		if (nameElement->GetText() == currentBranchName)
		{
			break;
		}
		branchElement = branchElement->NextSiblingElement();
	}
	auto branchStatusElement = branchElement->FirstChildElement("status");
	std::string statusString = branchStatusElement->GetText();
	if (statusString == "OK")
	{
		return EBranchStatus::OK;
	}
	if (statusString == "commiting")
	{
		return EBranchStatus::Commiting;
	}
	if (statusString == "pushing")
	{
		return EBranchStatus::Pushing;
	}
	if (statusString == "pulling")
	{
		return EBranchStatus::Pulling;
	}
	if (statusString == "merging")
	{
		return EBranchStatus::Merging;
	}
	if (statusString == "changing")
	{
		return EBranchStatus::Changing;
	}
	return EBranchStatus::Unknown;
}

void XMLModule::InitializeCheckOutFile()
{
	tinyxml2::XMLDocument doc;
	auto head = doc.NewElement("checkout");
	auto filesElement = doc.NewElement("files");
	filesElement->SetAttribute("count", 0);
	auto foldersElement = doc.NewElement("folders");
	foldersElement->SetAttribute("count", 0);
	head->InsertEndChild(filesElement);
	head->InsertEndChild(foldersElement);
	doc.InsertEndChild(head);
	if (doc.SaveFile(CheckOutFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE CHECKOUT FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::InitializeFilesStringPaths()
{
	ProjectFolderStringPath = GC->System->ProjectFolderPath.string() + "\\.FilesControl";
	IgnoreFileStringPath    = ProjectFolderStringPath                + "\\ignore.xml";
	StatusFileStringPath    = ProjectFolderStringPath                + "\\status.xml";
	CheckOutFileStringPath  = ProjectFolderStringPath                + "\\checkout.xml";
}

void XMLModule::IncreeseCountOfIntAttribute(tinyxml2::XMLElement* element)
{
	auto currentCount = element->IntAttribute("count");
	element->SetAttribute("count", ++currentCount);
}

void XMLModule::InitializeBranchFile(std::string branchName, std::string version, bool bCurrent)
{
	auto pathToBranchFile = ProjectFolderStringPath + "\\branches\\" + branchName + "\\" + version +"\\" + "branch_status.xml";
	tinyxml2::XMLDocument doc;
	auto head = doc.NewElement("branch");
	auto name = doc.NewElement("name");
	auto currentBranch = doc.NewElement("current_branch");
	auto filesElement = doc.NewElement("files");
	auto foldersElement = doc.NewElement("folders");
	name->SetText(branchName.c_str());
	currentBranch->SetText(bCurrent);
	head->InsertEndChild(name);
	head->InsertEndChild(currentBranch);
	head->InsertEndChild(filesElement);
	head->InsertEndChild(foldersElement);
	doc.InsertEndChild(head);
	if (doc.SaveFile(pathToBranchFile.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE BRANCH FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
	UpdateBranchFile(branchName, version, GetAllFiles(), GetAllFolders());
}

void XMLModule::UpdateBranchFile
(
	std::string branchName,
	std::string version,
	std::vector<std::filesystem::path> files,
	std::vector<std::filesystem::path> folders
)
{
	auto pathToBranchFile = ProjectFolderStringPath + "\\branches\\" + branchName + "\\" + version + "\\" + "branch_status.xml";
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(pathToBranchFile.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("NOT ABLE TO OPEN THE BRANCH FILE!", 
			  XMLModuleExeption::XMLModuleExeptionType::CouldNotOpenXMLFile);
	}
	auto head = doc.FirstChildElement();
	auto filesElement = head->FirstChildElement("files");
	for (const auto& file : files)
	{
		auto fileElement = doc.NewElement("file");
		fileElement->SetText(file.string().c_str());
		filesElement->InsertEndChild(fileElement);
	}
	auto foldersElement = head->FirstChildElement("folders");
	for (const auto& folder : folders)
	{
		auto folderElement = doc.NewElement("folder");
		folderElement->SetText(folder.string().c_str());
		foldersElement->InsertEndChild(folderElement);
	}
	if (doc.SaveFile(pathToBranchFile.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE BRANCH FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}
