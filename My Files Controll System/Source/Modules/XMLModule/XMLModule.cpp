#include "pch.h"
#include "XMLModule.h"
#include "Modules/SystemModule/SystemModule.h"


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
	InitializeStatusFile();
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
		auto lastTimeWrite = std::filesystem::last_write_time(filePath);
		fileElement->SetAttribute("changed_time", lastTimeWrite.time_since_epoch().count());
		filesElement->InsertEndChild(fileElement);
		IncreeseCountOfIntAttribute(filesElement);
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
		if (folderPath.string() == folderElement->GetText())
		{
			return true;
		}
		folderElement = foldersElement->NextSiblingElement();
	}
	return false;
}

const bool XMLModule::FindFileInIgnored(std::filesystem::path filePath) const
{
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
		fileElement = filesElement->NextSiblingElement();
	}
	return false;
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
	auto filesElement = doc.NewElement("files");
	filesElement->SetAttribute("count", 0);
	auto foldersElement = doc.NewElement("folders");
	foldersElement->SetAttribute("count", 0);
	head->InsertEndChild(filesElement);
	head->InsertEndChild(foldersElement);
	doc.InsertEndChild(head);
	if (doc.SaveFile(StatusFileStringPath.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
	{
		throw XMLModuleExeption("COULD NOT SAVE THE STATUS FILE!",
			XMLModuleExeption::XMLModuleExeptionType::CouldNotSaveXMLFile);
	}
}

void XMLModule::InitializeCheckOutFile()
{
	tinyxml2::XMLDocument doc;
	auto head = doc.NewElement("checkout");
	auto filesElement = doc.NewElement("files");
	filesElement->SetAttribute("count", 0);
	head->InsertEndChild(filesElement);
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
