#include "pch.h"
#include "Modules/ServerModule/public/ServerModule.h"
#include <fstream>

#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include "zip.h"

#include <functional>
#include <list>

#include <algorithm>
#include <queue>


void ServerModule::StartupModule()
{
    WORD Version = MAKEWORD(2, 2);
    WSAData Data;

    int Status = WSAStartup(Version, &Data);
    if (Status == -1)
    {
        std::cerr << "FAILED TO STARTUP SERVER!" << std::endl;
        throw std::runtime_error("FAILED TO STARTEDUP SERVER");
    }

    SOCKADDR_IN ServerSocketInfo;
    memset(&ServerSocketInfo, 0, sizeof(SOCKADDR_IN));

    int AddresSize = sizeof(ServerSocketInfo);
    ServerSocketInfo.sin_port = htons(13);
    ServerSocketInfo.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &ServerSocketInfo.sin_addr);

    SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(ServerSocket, (sockaddr*)&ServerSocketInfo, AddresSize);
    listen(ServerSocket, SOMAXCONN);
    std::thread Checker([&]() 
        {
            ClientsConnectionChecker(&ServerSocket, &ServerSocketInfo, AddresSize);
        });
    Threads.push_back(&Checker);

    for (auto tr : Threads)
    {
        tr->join();
    }
}

void ServerModule::Update()
{
}

void ServerModule::ShutdownModule()
{
    WSACleanup();
    for (auto tr : Threads)
    {
        tr->detach();
    }
}

EServerCommand ServerModule::ReciveCommand(UINT_PTR* ClientSocket)
{
    char command[SERVER_CLIENT_COMMAND_LENGH];
    recv(*ClientSocket, command, SERVER_CLIENT_COMMAND_LENGH, 0);
    std::string str = command;
    std::cout << str << std::endl;
    if (str == "ReciveDataFromClient")
    {
        return EServerCommand::ReciveDataFromClient;
    }
    if (str == "SendDataToClient")
    {
        return EServerCommand::SendDataToClient;
    }
    if (str == "StopRecivingFilesFromClient")
    {
        return EServerCommand::StopRecivingFilesFromClient;
    }
}

void ServerModule::HandleCommand(UINT_PTR* ClientSocket)
{
    static std::mutex mtx;
    mtx.lock();
    bool ShouldContinue = true;
    while (ShouldContinue)
    {
        auto CommandType = ReciveCommand(ClientSocket);
        switch (CommandType)
        {
        case EServerCommand::ReciveDataFromClient:
        {
            ShouldContinue = ReciveDataFromClient(ClientSocket);
        } break;
        case EServerCommand::StopRecivingFilesFromClient:
        {
            ShouldContinue = false;
        }break;
        }
    }
    mtx.unlock();
}

const bool ServerModule::ReciveDataFromClient(UINT_PTR* ClientSocket)
{
    char* FileSize = new char[RECIVE_FILE_SIZE];
    char* FileName = new char[RECIVE_FILE_NAME_SIZE];
    recv(*ClientSocket, FileSize, RECIVE_FILE_SIZE, 0);
    recv(*ClientSocket, FileName, RECIVE_FILE_NAME_SIZE, 0);
    if (std::string(FileName) == "")
    {
        return false;
    }
    const int IntFileSize = std::atoi(FileSize);
    char* FileData = new char[IntFileSize];
    recv(*ClientSocket, FileData, IntFileSize, 0);
    std::cout << "FileSize: " << FileSize << std::endl;
    std::cout << "FileName: " << FileName << std::endl;
    std::cout << "FileData: " << FileData << std::endl;
    std::ofstream out(FileName, std::ios::binary | std::ios::out);
    out.write(FileData, IntFileSize);
    out.close();
    delete[] FileSize;
    delete[] FileName;
    delete[] FileData;
    return true;
}


void ServerModule::ClientsConnectionChecker(UINT_PTR* ServerSocket, sockaddr_in* ServerSocketInfo, int AddresSize)
{
    while (true)
    {
        SOCKET ClientSocket = accept(*ServerSocket, (sockaddr*)ServerSocketInfo, &AddresSize);
        if (ClientSocket != 0)
        {
            std::cout << "Connected at " << std::this_thread::get_id() << " thread!" << std::endl;
            UserConnection uc;
            uc.UserSocket = &ClientSocket;
            uc.Handle = std::async(std::launch::async, &ServerModule::HandleCommand, this, &ClientSocket);
            uc.Handle.get();
        }
    }
}

