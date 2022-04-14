#include "Internet/public/InternetComponent.h"
#include <iostream>
#include <urlmon.h>
#include <tchar.h>
#include <thread>
#include <filesystem>


#pragma comment (lib, "urlmon.lib")

void InternetComponent::StartupModule()
{
}

void InternetComponent::DownloadFile(std::filesystem::path FilePath, std::string FilePathOnServer)
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
