#pragma once
#include "Modules/IBaseModule.h"

#include <thread>
#include <mutex>
#include <shared_mutex>

#include <basetsd.h>
#include <functional>
#include <future>


#define SERVER_CLIENT_COMMAND_LENGH 32
#define RECIVE_FILE_SIZE 32
#define RECIVE_FILE_NAME_SIZE 64


#define RECIVE_DATA_FROM_CLIENT_KEY 0x000001
#define SEND_DATA_TO_CLIENT_KEY 0x000002
#define STOP_RECIVING_FILES_FROM_CLIENT_KEY 0x000003


enum class EServerCommand
{
	ReciveDataFromClient = RECIVE_DATA_FROM_CLIENT_KEY,
	SendDataToClient = SEND_DATA_TO_CLIENT_KEY,
	StopRecivingFilesFromClient = STOP_RECIVING_FILES_FROM_CLIENT_KEY
};


class ServerModule : public virtual IBaseModule
{
private:
	struct UserConnection
	{
		UINT_PTR* UserSocket;
		std::future<void> Handle;
		friend const bool operator==(const UserConnection& left, const UserConnection& right)
		{
			if (!left.UserSocket || !right.UserSocket)
			{
				return false;
			}
			if (*left.UserSocket == *right.UserSocket)
			{
				return true;
			}
			return false;
		}
	};
public:
	explicit ServerModule(std::string ModuleName) noexcept : Super(ModuleName) {};
public:
	virtual void StartupModule() override;
	virtual void Update() override;
	virtual void ShutdownModule() override;
protected:
	virtual EServerCommand ReciveCommand(UINT_PTR* ClientSocket);
	virtual void HandleCommand(UINT_PTR* ClientSocket);
	virtual const bool ReciveDataFromClient(UINT_PTR* ClientSocket);
	virtual void ClientsConnectionChecker(UINT_PTR* ServerSocket, struct sockaddr_in* ServerSocketInfo, int AddresSize);
private:
	std::vector<std::thread*> Threads;
	std::vector<UserConnection> UserConnections;
};