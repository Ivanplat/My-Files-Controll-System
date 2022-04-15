#include "pch.h"
#include "Modules/FilesModule/public/FilesModule.h"
#include <fstream>

#include "zip.h"

void FilesModule::StartupModule()
{
}

void FilesModule::Update()
{
}

void FilesModule::ShutdownModule()
{
}

void FilesModule::CreateFile(std::filesystem::path Path)
{
	std::ofstream out(Path, std::ios::app);
	out.close();
}

void FilesModule::DeleteFile(std::filesystem::path Path)
{
	std::remove(Path.string().c_str());
}

void FilesModule::CreateZipFromFile(std::filesystem::path Path)
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
	zip_close(z);
}

void FilesModule::CrateDirectory(std::filesystem::path Path)
{
	std::filesystem::create_directory(Path);
}

void FilesModule::DeleteDirectory(std::filesystem::path Path)
{
	std::filesystem::remove_all(Path);
}

std::string FilesModule::GetFileNameFromPath(std::filesystem::path Path)
{
	//GC->Log->PrintToLog("FilesModule::GetFileNameFromPath()");
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

const bool FilesModule::CheckDirectory(std::filesystem::path Path) const
{
	return std::filesystem::is_directory(Path);
}

const bool FilesModule::CheckFile(std::filesystem::path Path) const
{
	return std::filesystem::is_block_file(Path) || std::filesystem::is_character_file(Path) || std::filesystem::is_regular_file(Path);
}
