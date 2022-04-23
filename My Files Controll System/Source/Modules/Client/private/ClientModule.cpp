#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include "pch.h"
#include "Modules/Client/public/ClientModule.h"

#include "boost/asio.hpp"

#include <tchar.h>

#include <urlmon.h>

#include <WS2tcpip.h>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "WS2_32.lib")


void ClientModule::SendFiles(UINT_PTR* sock)
{
    std::atomic_int CountOfFilesInOrder = FilesStack.size() - 1;
    while (true)
    {
        auto FileName = FilesStack.top();
        if (FileName == "")
        {
            send(*sock, std::string("StopRecivingFilesFromClient").c_str(), 32, 0);
            break;
        }
        FilesStack.pop();
        std::ifstream in(FileName, std::ios::in | std::ios::binary);
        send(*sock, std::string("ReciveDataFromClient").c_str(), 32, 0);
        std::cout << "ReciveDataFromClient\n";

        if (in.is_open())
        {
            unsigned long long fileSize = std::filesystem::file_size(FileName) + 1;
            char* bytes = new char[fileSize];

            in.read(bytes, fileSize);
            std::cout << "Uploading " << FileName << std::endl;
            auto File = GC->Files->GetFileNameFromPath(FileName);
            send(*sock, std::to_string(fileSize).c_str(), 32, 0);
            send(*sock, File.c_str(), 64, 0);
            send(*sock, bytes, fileSize, 0);
            auto TrimmedFileName = GC->Files->GetFileNameFromPath(FileName);
            GC->XML->UpdateSendDataStatusForFile(TrimmedFileName);
            auto t1 = (static_cast<float>(FilesStack.size() - 1) / static_cast<float>(CountOfFilesInOrder));
            std::cout << "Files uploaded: " <<  (1.0f - t1) * 100.0f <<"%"<< std::endl;
            delete[] bytes;
        }
    }
    std::cout << "All files have been uploaded!"<< std::endl;
}


void ClientModule::StartupModule()
{
    std::thread* ClientThread = new std::thread([&]() 
        {
            FilesStack.push("");

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
                std::string file2 = "C:\\Users\\EventGraph\\Downloads\\Методичка_1-1.pdf";
                GC->Files->CreateFilesStack({file, file2});
                std::cout << "Files in order: " << FilesStack.size() - 1 << std::endl;
                std::cout << "Files uploaded: 0%" << std::endl;
                SendFiles(&sock);
            }
            else
            {
                std::cout << "ERROR: " << WSAGetLastError() << std::endl;
            }

            while (true);

            std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    GC->System->Threads.insert(std::pair<const std::string, std::thread*>(std::string("ClientThread"), ClientThread));
}

void ClientModule::Update()
{
}

void ClientModule::ShutdownModule()
{
}

void ClientModule::DownloadFile(std::filesystem::path FilePath, std::string FilePathOnServer)
{
    std::thread tr([&]() 
    {
        HRESULT hr = URLDownloadToFileA(NULL, FilePathOnServer.c_str(), FilePath.string().c_str(), 0, 0);

        if (hr == S_OK)
        {
            std::cout << "‘айл успешно скачан, открываем его\n";
        }
    });
    tr.join();
}
