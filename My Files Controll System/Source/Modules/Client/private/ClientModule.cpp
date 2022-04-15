#include "pch.h"
#include "Modules/Client/public/ClientModule.h"
#include <urlmon.h>
#include <tchar.h>


#pragma comment (lib, "urlmon.lib")

void ClientModule::StartupModule()
{
}

void ClientModule::Update()
{
}

void ClientModule::DownloadFile(std::filesystem::path FilePath, std::string FilePathOnServer)
{
    std::thread tr([&]() 
    {
        HRESULT hr = URLDownloadToFileA(NULL, FilePathOnServer.c_str(), FilePath.string().c_str(), 0, 0);

        if (hr == S_OK)
        {
            std::cout << "Файл успешно скачан, открываем его\n";
        }
    });
    tr.join();
}
