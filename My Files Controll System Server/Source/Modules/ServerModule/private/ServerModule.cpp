#include "pch.h"
#include "Modules/ServerModule/public/ServerModule.h"
#include <fstream>

#include "zip.h"

#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")


#include <functional>
#include <list>


#include <thread>
#include <mutex>
#include <shared_mutex>

void ServerModule::StartupModule()
{
	WSADATA wsadata;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsadata);
	if (wsOK != 0)
	{
		GC->Log->PrintToConsole("Can not start up");
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		GC->Log->PrintToConsole("Invalid socket");
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	listen(listening, SOMAXCONN);


	sockaddr_in client;
	int client_size = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &client_size);

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << "Connected on port: " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << "Connected on port: " << htons(client.sin_port) << std::endl;
	}

	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);

		int ButesRecived = recv(clientSocket, buf, 4096, 0);
		/*if (ButesRecived == SOCKET_ERROR)
		{
			GC->Log->PrintToConsole("Error in socket");
			break;
		}*/
		if (ButesRecived == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			break;
		}
		send(clientSocket, buf, ButesRecived + 1, 0);
	}
	closesocket(clientSocket);
	closesocket(listening);

	WSACleanup();
}

void ServerModule::Update()
{
}

void ServerModule::ShutdownModule()
{
}

