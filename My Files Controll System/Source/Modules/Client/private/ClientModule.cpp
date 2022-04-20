#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include "pch.h"
#include "Modules/Client/public/ClientModule.h"

#include "boost/asio.hpp"

#include <tchar.h>

#include <urlmon.h>

#include <WS2tcpip.h>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "WS2_32.lib")


void SendData(SOCKET* sock, const std::string& FileName)
{
    std::ifstream in(FileName, std::ios::in | std::ios::binary);

    if (in.is_open())
    {
        unsigned long long fileSize = std::filesystem::file_size(FileName) + 1;

        char* bytes = new char[fileSize];

        in.read(bytes, fileSize);

        auto File = GC->Files->GetFileNameFromPath(FileName);


        send(*sock, std::to_string(fileSize).c_str(), 16, 0);
        send(*sock, File.c_str(), 32, 0);
        send(*sock, bytes, fileSize, 0);

        delete[] bytes;
    }
}


void ClientModule::StartupModule()
{
  
    WORD vers = MAKEWORD(2, 2);
    WSAData data;

    WSAStartup(vers, &data);

    SOCKADDR_IN sock_info;
    memset(&sock_info, 0, sizeof(SOCKADDR_IN));

    int addr_size = sizeof(sock_info);
    sock_info.sin_port = htons(13);
    sock_info.sin_family = AF_INET;
    sock_info.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cout << "INVALID SOCKET" << std::endl;
    }
    if (connect(sock, (sockaddr*)&sock_info, sizeof(sock_info)) == 0)
    {
        std::cout << "Connected!" << std::endl;
        std::string file = "E:\\Unreal Engine\\UE_4.26\\Engine\\Binaries\\Win64\\UE4Editor.exe";
        SendData(&sock, file);
    }
    else
    {
        std::cout << "ERROR: "<< WSAGetLastError() << std::endl;
    }

    while (true);

    std::this_thread::sleep_for(std::chrono::seconds(1));
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
            std::cout << "Сайл успешно скачан, открываем его\n";
        }
    });
    tr.join();
}
