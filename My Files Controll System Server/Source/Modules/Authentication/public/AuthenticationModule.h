#pragma once

#include "Modules/IBaseModule.h"
#include "AES/AES.h"


class AuthenticationModule : public virtual IBaseModule
{
public:
	explicit AuthenticationModule(std::string ModuleName) noexcept;
public:
	virtual void StartupModule() override;
	virtual void Update() override;
	virtual void ShutdownModule() override;
	virtual void SendKeyToClient();
private:
	std::unique_ptr<AES> Aes;
};