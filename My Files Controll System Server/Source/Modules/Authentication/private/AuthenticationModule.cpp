#include "pch.h"
#include "Modules/Authentication/public/AuthenticationModule.h"


AuthenticationModule::AuthenticationModule(std::string ModuleName) noexcept : Super(ModuleName)
{
	Aes = std::unique_ptr<AES>(new AES(AESKeyLength::AES_256));
}

void AuthenticationModule::StartupModule()
{

}

void AuthenticationModule::Update()
{

}

void AuthenticationModule::ShutdownModule()
{

}

void AuthenticationModule::SendKeyToClient()
{
	auto Key = Aes->KeyGenerator();

}
