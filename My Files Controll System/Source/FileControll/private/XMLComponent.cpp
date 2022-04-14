#include "FileControll/public/XMLComponent.h"

#include "FileControll/public/FileControll.h"

#include <iostream>

using namespace tinyxml2;

void XMLComponent::StartupModule()
{
	IgnoreDocument = std::unique_ptr<XMLDocument>(new XMLDocument());
	InitStatusFile();
	InitIgnoreFile();
	InitCheckoutFile();
}

void XMLComponent::InitIgnoreFile()
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\ignore.xml";

	if (!FileControllComponent::CheckFile(FilePath))
	{
		FileControllComponent::CreateFile(FilePath);
		IgnoreDocument->LoadFile(FilePath.c_str());

		XMLElement* Root = IgnoreDocument->NewElement("Ignore");
		Root->SetAttribute("Version", "");
		Root->SetAttribute("Platform", "");
		IgnoreDocument->InsertFirstChild(Root);

		auto DirectoriesElement = IgnoreDocument->NewElement("DIRECTORIES");
		auto DirectoriesSum = IgnoreDocument->NewElement("DIRECTORIES-CHECK-SUM");
		DirectoriesSum->SetAttribute("Count", 0);
		DirectoriesElement->InsertEndChild(DirectoriesSum);

		Root->InsertEndChild(DirectoriesElement);
		auto FilesElement = IgnoreDocument->NewElement("FILES");
		auto FilesSum = IgnoreDocument->NewElement("FILES-CHECK-SUM");
		FilesSum->SetAttribute("Count", 0);
		FilesElement->InsertEndChild(FilesSum);

		Root->InsertEndChild(FilesElement);
		IgnoreDocument->SaveFile(FilePath.c_str());
	}
}

void XMLComponent::InitCheckoutFile()
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\checkout.xml";

	if (!FileControllComponent::CheckFile(FilePath))
	{
		FileControllComponent::CreateFile(FilePath);

		XMLDocument Doc;
		Doc.LoadFile(FilePath.c_str());

		XMLElement* Root = Doc.NewElement("Checkout");
		Root->SetAttribute("Version", "");
		Root->SetAttribute("Platform", "");
		Doc.InsertFirstChild(Root);
		auto DirectoriesElement = Doc.NewElement("DIRECTORIES");
		Root->InsertEndChild(DirectoriesElement);
		auto FilesElement = Doc.NewElement("FILES");

		Root->InsertEndChild(FilesElement);
		Doc.SaveFile(FilePath.c_str());
	}
}

void XMLComponent::InitStatusFile()
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\status.xml";
	if (!FileControllComponent::CheckFile(FilePath))
	{
		FileControllComponent::CreateFile(FilePath);

		XMLDocument Doc;
		Doc.LoadFile(FilePath.c_str());

		XMLElement* Root = Doc.NewElement("Status");
		Root->SetAttribute("Version", "");
		Root->SetAttribute("Platform", "");
		Doc.InsertFirstChild(Root);

		auto InitializedElement = Doc.NewElement("Initialized");
		InitializedElement->SetAttribute("value", true);

		Root->InsertEndChild(InitializedElement);
		Doc.SaveFile(FilePath.c_str());
	}
}

void XMLComponent::AddFileToIgnore(std::filesystem::path Path, std::string Hash)
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\ignore.xml";

	if (FileControllComponent::CheckFile(FilePath))
	{
		auto Root = IgnoreDocument->FirstChildElement();
		if (Root)
		{
			auto FilesElement = Root->FirstChildElement("FILES");
			auto FileInfo = IgnoreDocument->NewElement("File");
			FileInfo->SetAttribute("Path", Path.string().c_str());
			FileInfo->SetAttribute("Hash", Hash.c_str());
			FileInfo->SetText(GetFileNameFromPath(Path).c_str());
			FilesElement->InsertFirstChild(FileInfo);
		}
	}
}

void XMLComponent::AddDirectoryToIgnore(std::filesystem::path Path)
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\ignore.xml";

	if (FileControllComponent::CheckFile(FilePath))
	{
		auto Root = IgnoreDocument->FirstChildElement();
		if (Root)
		{
			auto DirectoriesElement = Root->FirstChildElement("DIRECTORIES");
			auto DirectoryInfo = IgnoreDocument->NewElement("Directory");
			auto DirectoriesSum = DirectoriesElement->FirstChildElement("DIRECTORIES-CHECK-SUM");
			if (DirectoriesSum)
			{
				auto CurrentValue = DirectoriesSum->FindAttribute("Count")->IntValue();
				DirectoriesSum->SetAttribute("Count", ++CurrentValue);
			}
			DirectoryInfo->SetText(GetFileNameFromPath(Path).c_str());
			DirectoryInfo->SetAttribute("Path", Path.string().c_str());
			DirectoriesElement->InsertEndChild(DirectoryInfo);
		}
	}
}

void XMLComponent::RemoveFileFromIgnore(std::filesystem::path Path)
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\ignore.xml";

	if (FileControllComponent::CheckFile(FilePath))
	{
		auto Root = IgnoreDocument->FirstChildElement();
		if (Root)
		{
			auto FilesElement = Root->FirstChildElement("FILES");
			auto FileInfo = FilesElement->FirstChildElement();
			if (FileInfo)
			{
				while (FileInfo->FindAttribute("Path")->Value() != Path.string())
				{
					FileInfo = FilesElement->NextSiblingElement("File");
				}
				if (FileInfo)
				{
					FilesElement->DeleteChild(FileInfo);
				}
			}
		}
	}
}

void XMLComponent::RemoveDirectoryFromIgnore(std::filesystem::path Path)
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\ignore.xml";

	if (FileControllComponent::CheckFile(FilePath))
	{
		auto Root = IgnoreDocument->FirstChildElement();
		if (Root)
		{
			auto DirectoriesElement = Root->FirstChildElement("DIRECTORIES");
			auto DirectoryInfo = DirectoriesElement->FirstChildElement();
			if (DirectoryInfo)
			{
				while (DirectoryInfo->FindAttribute("Path")->Value() != Path.string())
				{
					DirectoryInfo = DirectoriesElement->NextSiblingElement("Directory");
				}
				if (DirectoryInfo)
				{
					DirectoriesElement->DeleteChild(DirectoryInfo);
				}
			}
		}
	}
}

void XMLComponent::SaveIgnoreDocument()
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\ignore.xml";
	if (!IgnoreDocument->Error())
	{
		IgnoreDocument->SaveFile(FilePath.c_str());
	}
}

void XMLComponent::LoadIgnoreDocument()
{
	auto FilePath = FileControllComponent::GetRootDirectory() + "\\FilesControll\\ignore.xml";
	std::cout<<IgnoreDocument->LoadFile(FilePath.c_str());
}

std::string XMLComponent::GetFileNameFromPath(std::filesystem::path Path)
{
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
