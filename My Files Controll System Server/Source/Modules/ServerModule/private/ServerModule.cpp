#include "pch.h"
#include "Modules/ServerModule/public/ServerModule.h"
#include <fstream>

#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include "zip.h"

#include <functional>
#include <list>


#include <thread>
#include <mutex>
#include <shared_mutex>

#include <algorithm>

struct UserConnection
{
	unsigned long user_id;
	std::string user_name;
};


void recive_file(SOCKET* sock)
{
    char file_size_str[16];
    char file_name[32];

    recv(*sock, file_size_str, 16, 0);
    int file_size = atoi(file_size_str);
    char* bytes = new char[file_size];

    recv(*sock, file_name, 32, 0);
    std::fstream file(file_name, std::ios::out | std::ios::binary);

    if (file.is_open())
    {
        recv(*sock, bytes, file_size, 0);
        std::cout << bytes;
        file.write(bytes, file_size);
        file.close();
    }
    delete[] bytes;
}

void ServerModule::StartupModule()
{
    WORD vers = MAKEWORD(2, 2);
    WSAData data;

    WSAStartup(vers, &data);

    SOCKADDR_IN sock_info;
    memset(&sock_info, 0, sizeof(SOCKADDR_IN));

    int addr_size = sizeof(sock_info);
    sock_info.sin_port = htons(13);
    sock_info.sin_family = AF_INET;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    bind(sock, (sockaddr*)&sock_info, sizeof(sock_info));
    listen(sock, SOMAXCONN);

    SOCKET s_connect = accept(sock, (sockaddr*)&sock_info, &addr_size);
    if (s_connect != 0)
    {
        std::cout << "Connected 1!" << std::endl;
        recive_file(&s_connect);
    }

}

void ServerModule::Update()
{
}

void ServerModule::ShutdownModule()
{
}

