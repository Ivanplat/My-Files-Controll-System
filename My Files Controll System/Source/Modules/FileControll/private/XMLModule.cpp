#include "pch.h"

using namespace tinyxml2;

void XMLModule::StartupModule()
{
	GC->Log->PrintToLog("XMLComponent::StartupModule()");
	IgnoreDocument = std::unique_ptr<XMLDocument>(new XMLDocument());
	//InitStatusFile();
	//InitIgnoreFile();
	//InitCheckoutFile();
	GC->Log->PrintToLog("XMLComponent started up");
}

void XMLModule::Update()
{
	GC->Log->PrintToLog("XMLComponent::Update()");
	GC->Files->Update();
}

void XMLModule::ShutdownModule()
{
}

void XMLModule::InitIgnoreFile()
{
	GC->Log->PrintToLog("XMLComponent::InitIgnoreFile()");
	auto Path = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";

	if (!GC->Files->CheckFile(Path))
	{
		//LoadIgnoreDocument();
		GC->Files->CreateFile(Path);
		IgnoreDocument->LoadFile(Path.c_str());

		XMLElement* Root = IgnoreDocument->NewElement("Ignore");
		Root->SetAttribute("Version", "");
		Root->SetAttribute("Platform", "");
		IgnoreDocument->InsertFirstChild(Root);

		auto DirectoriesElement = IgnoreDocument->NewElement("DIRECTORIES");
		DirectoriesElement->SetAttribute("Count", 0);

		auto DirectoryElement = IgnoreDocument->NewElement("Directory");
		DirectoryElement->SetText((GC->Files->GetRootDirectory().string() + "\\FilesControll").c_str());

		DirectoriesElement->InsertEndChild(DirectoryElement);

		Root->InsertEndChild(DirectoriesElement);
		auto FilesElement = IgnoreDocument->NewElement("FILES");
		FilesElement->SetAttribute("Count", 0);

		Root->InsertEndChild(FilesElement);
		IgnoreDocument->SaveFile(Path.c_str());

	}
}

void XMLModule::InitCheckoutFile()
{
	GC->Log->PrintToLog("XMLComponent::InitCheckoutFile()");
	auto Path = GC->Files->GetRootDirectory().string() + "\\FilesControll\\checkout.xml";

	if (!GC->Files->CheckFile(Path))
	{
		GC->Files->CreateFile(Path);

		XMLDocument Doc;
		Doc.LoadFile(Path.c_str());

		XMLElement* Root = Doc.NewElement("Checkout");
		Root->SetAttribute("Version", "");
		Root->SetAttribute("Platform", "");
		Doc.InsertFirstChild(Root);
		auto DirectoriesElement = Doc.NewElement("DIRECTORIES");
		Root->InsertEndChild(DirectoriesElement);
		auto FilesElement = Doc.NewElement("FILES");

		Root->InsertEndChild(FilesElement);
		Doc.SaveFile(Path.c_str());
	}
}

void XMLModule::InitStatusFile(std::string RepositoryName)
{
	GC->Log->PrintToLog("XMLComponent::InitStatusFile()");
	auto Path = GC->Files->GetRootDirectory().string() + "\\FilesControll\\status.xml";

	if (!GC->Files->CheckFile(Path))
	{
		GC->Files->CreateFile(Path);

		XMLDocument Doc;
		Doc.LoadFile(Path.c_str());

		XMLElement* Root = Doc.NewElement("Status");
		Root->SetAttribute("Version", "");
		Root->SetAttribute("Platform", "");
		Doc.InsertFirstChild(Root);

		auto InitializedElement = Doc.NewElement("Initialized");
		InitializedElement->SetAttribute("value", true);

		auto RepositoryNameElement = Doc.NewElement("RepositoryName");
		RepositoryNameElement->SetText(RepositoryName.c_str());

		auto BranchesElement = Doc.NewElement("Branches");
		BranchesElement->SetAttribute("Count", 0);

		auto PublishedElement = Doc.NewElement("Published");
		PublishedElement->SetText("");
		PublishedElement->BoolAttribute("value", false);

		auto RootDirectoryElement = Doc.NewElement("RootDirectory");
		RootDirectoryElement->SetText(GC->Files->GetRootDirectory().string().c_str());

		auto InitialBranch = Doc.NewElement("Branch");
		InitialBranch->SetAttribute("CurrentVerison", "123");
		InitialBranch->SetText("main");

		BranchesElement->InsertEndChild(InitialBranch);
		Root->InsertEndChild(InitializedElement);
		Root->InsertEndChild(RepositoryNameElement);
		Root->InsertEndChild(RootDirectoryElement);
		Root->InsertEndChild(PublishedElement);
		Root->InsertEndChild(BranchesElement);
		Doc.SaveFile(Path.c_str());
	}
}

void XMLModule::InitBranchVersionInfoFile(std::string BranchName, std::string VersionString)
{
	GC->Log->PrintToLog("XMLComponent::InitBranchVersionInfoFile()");
	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\branches\\"+ BranchName +"\\"+ VersionString+"\\branch-version-info.xml";

	if (!GC->Files->CheckFile(FilePath))
	{
		GC->Files->CreateFile(FilePath);

		XMLDocument Doc;
		Doc.LoadFile(FilePath.c_str());

		XMLElement* Root = Doc.NewElement("branch-version-info");
		Root->SetAttribute("Version", VersionString.c_str());
		Root->SetAttribute("Current", true);
		Doc.InsertFirstChild(Root);

		auto RepositoryNameElement = Doc.NewElement("BranchName");
		RepositoryNameElement->SetText(BranchName.c_str());

		auto FilesElement = Doc.NewElement("FILES");
		FilesElement->SetAttribute("Count", 0);

		Root->InsertEndChild(RepositoryNameElement);
		Root->InsertEndChild(FilesElement);
		Doc.SaveFile(FilePath.c_str());
	}
}

void XMLModule::AddFileToIgnore(std::filesystem::path Path)
{
	GC->Log->PrintToLog("XMLComponent::AddFileToIgnore()");
	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";

	if (GC->Files->CheckFile(FilePath))
	{
		LoadIgnoreDocument();
		auto Root = IgnoreDocument->FirstChildElement();
		if (Root)
		{
			auto Hash = GC->Files->HashOfBinaryFile(Path);
			auto FilesElement = Root->FirstChildElement("FILES");
			auto CurrentValue = FilesElement->FindAttribute("Count")->IntValue();
			FilesElement->SetAttribute("Count", ++CurrentValue);
			auto FileInfo = IgnoreDocument->NewElement("File");
			FileInfo->SetAttribute("Path", Path.string().c_str());
			FileInfo->SetAttribute("Hash", Hash.c_str());
			FileInfo->SetText(GC->Files->GetFileNameFromPath(Path).c_str());
			FilesElement->InsertFirstChild(FileInfo);
		}
		SaveIgnoreDocument();
		Update();
	}
}

void XMLModule::AddDirectoryToIgnore(std::filesystem::path Path)
{
	GC->Log->PrintToLog("XMLComponent::AddDirectoryToIgnore()");
	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";

	if (GC->Files->CheckFile(FilePath))
	{
		LoadIgnoreDocument();
		auto Root = IgnoreDocument->FirstChildElement();
		if (Root)
		{
			auto DirectoriesElement = Root->FirstChildElement("DIRECTORIES");
			auto DirectoryInfo = IgnoreDocument->NewElement("Directory");
		    auto CurrentValue = DirectoriesElement->FindAttribute("Count")->IntValue();

			DirectoriesElement->SetAttribute("Count", ++CurrentValue);
			DirectoryInfo->SetText(GC->Files->GetFileNameFromPath(Path).c_str());
			DirectoryInfo->SetAttribute("Path", Path.string().c_str());
			DirectoriesElement->InsertEndChild(DirectoryInfo);
		}
		SaveIgnoreDocument();
		Update();
	}
}

void XMLModule::RemoveFileFromIgnore(std::filesystem::path Path)
{
	GC->Log->PrintToLog("XMLComponent::RemoveFileFromIgnore()");
	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";

	if (GC->Files->CheckFile(FilePath))
	{
		LoadIgnoreDocument();
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
					auto CurrentValue = FilesElement->FindAttribute("Count")->IntValue();
					FilesElement->SetAttribute("Count", --CurrentValue);
					FilesElement->DeleteChild(FileInfo);
				}
			}
		}
		SaveIgnoreDocument();
		Update();
	}
}

void XMLModule::RemoveDirectoryFromIgnore(std::filesystem::path Path)
{
	GC->Log->PrintToLog("XMLComponent::RemoveDirectoryFromIgnore()");
	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";

	if (GC->Files->CheckFile(FilePath))
	{
		LoadIgnoreDocument();
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
					auto CurrentValue = DirectoriesElement->FindAttribute("Count")->IntValue();
					DirectoriesElement->SetAttribute("Count", --CurrentValue);
					DirectoriesElement->DeleteChild(DirectoryInfo);
				}
			}
		}
		SaveIgnoreDocument();
		Update();
	}
}

void XMLModule::CreateSendDataStatusFile()
{
	GC->Log->PrintToLog("XMLComponent::CreateSendDataStatusFile()");
	auto Path = GC->Files->GetRootDirectory().string() + "\\FilesControll\\SendDataStatus.xml";

	if (!GC->Files->CheckFile(Path))
	{
		GC->Files->CreateFile(Path);

		XMLDocument Doc;
		Doc.LoadFile(Path.c_str());

		XMLElement* Root = Doc.NewElement("Status");
		Root->SetAttribute("Version", "");
		Root->SetAttribute("Platform", "");
		Doc.InsertFirstChild(Root);

		auto FilesElement = Doc.NewElement("Files");
		auto cpy = GC->Client->FilesStack;
		for (size_t i = 0; i < cpy.size(); i++)
		{
			auto FileElement = Doc.NewElement("File");
			auto FilePath = cpy.top();
			auto FileName = GC->Files->GetFileNameFromPath(FilePath);
			FileElement->SetText(FileName.c_str());
			FileElement->SetAttribute("Path", FilePath.c_str());
			FileElement->SetAttribute("Send", false);
			FilesElement->InsertEndChild(FileElement);
			cpy.pop();
		}
		Root->InsertEndChild(FilesElement);

		Doc.SaveFile(Path.c_str());
	}
}

void XMLModule::UpdateSendDataStatusForFile(std::string FileName)
{
	GC->Log->PrintToLog("XMLComponent::UpdateSendDataStatusForFile()");
	auto Path = GC->Files->GetRootDirectory().string() + "\\FilesControll\\SendDataStatus.xml";

	if (GC->Files->CheckFile(Path))
	{
		XMLDocument Doc;
		Doc.LoadFile(Path.c_str());
		XMLElement* Root = Doc.FirstChildElement();
		auto FilesElement = Root->FirstChildElement("Files");
		auto FileElement = FilesElement->FirstChildElement("File");
		if (FileElement)
		{
			while (FileElement->GetText() != FileName)
			{
				FileElement = FileElement->NextSiblingElement("File");
			}
			if (FileElement)
			{
				FileElement->SetAttribute("Send", true);
			}
		}
		Doc.SaveFile(Path.c_str());
	}
}

void XMLModule::SaveIgnoreDocument()
{
	GC->Log->PrintToLog("XMLComponent::SaveIgnoreDocument()");
	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";
	if (!IgnoreDocument->Error())
	{
		IgnoreDocument->SaveFile(FilePath.c_str());
	}
}

void XMLModule::LoadIgnoreDocument()
{
	GC->Log->PrintToLog("XMLComponent::LoadIgnoreDocument()");
	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";
	std::cout<<IgnoreDocument->LoadFile(FilePath.c_str());
}

void XMLModule::InitializeFiles(std::filesystem::path RepositoryPath, std::string RepositoryName, std::string VersionString)
{
	auto FileControllPath = RepositoryPath.string() + "\\FilesControll";

	InitCheckoutFile();
	InitIgnoreFile();
	InitStatusFile(RepositoryName);
	InitBranchVersionInfoFile("main", VersionString);
}

std::vector<std::filesystem::path> XMLModule::GetIgnoredDirectories()
{
	GC->Log->PrintToLog("XMLComponent::GetIgnoredDirectories()");
	std::vector<std::filesystem::path> result;

	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";

	if (GC->Files->CheckFile(FilePath))
	{
		LoadIgnoreDocument();
		auto Root = IgnoreDocument->FirstChildElement();
		if (Root)
		{
			auto DirectoriesElement = Root->FirstChildElement("DIRECTORIES");
			auto Count = DirectoriesElement->FindAttribute("Count")->IntValue();
			auto DirectoryInfo = DirectoriesElement->FirstChildElement();
			for (size_t i = 0; i < Count; i++)
			{
				if (DirectoryInfo)
				{
					auto Path = DirectoryInfo->FindAttribute("Path")->Value();
					result.push_back(Path);
					DirectoryInfo = DirectoryInfo->NextSiblingElement("Directory");
				}
			}
		}
		SaveIgnoreDocument();
	}

	return result;
}

std::vector<std::pair<std::filesystem::path, std::string>> XMLModule::GetIgnoredFiles()
{
	GC->Log->PrintToLog("XMLComponent::GetIgnoredFiles()");
	std::vector<std::pair<std::filesystem::path, std::string>> result;

	auto FilePath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\ignore.xml";

	if (GC->Files->CheckFile(FilePath))
	{
		LoadIgnoreDocument();
		auto Root = IgnoreDocument->FirstChildElement();
		if (Root)
		{
			auto DirectoriesElement = Root->FirstChildElement("FILES");
			auto Count = DirectoriesElement->FindAttribute("Count")->IntValue();
			auto DirectoryInfo = DirectoriesElement->FirstChildElement();
			for (size_t i = 0; i < Count; i++)
			{
				if (DirectoryInfo)
				{
					auto Path = DirectoryInfo->FindAttribute("Path")->Value();
					auto Hash = DirectoryInfo->FindAttribute("Hash")->Value();
					std::pair<std::filesystem::path, std::string> temp(Path, Hash);
					result.push_back(temp);
					DirectoryInfo = DirectoryInfo->NextSiblingElement("File");
				}
			}
		}
		SaveIgnoreDocument();
	}

	return result;
}
