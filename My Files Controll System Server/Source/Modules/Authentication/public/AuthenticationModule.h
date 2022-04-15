#pragma once

#include "Modules/IBaseModule.h"


class AuthenticationModule : public virtual IBaseModule
{
public:
	explicit AuthenticationModule(std::string ModuleName) noexcept : Super(ModuleName) {};
public:
	virtual void StartupModule() override;
	virtual void Update() override;
	virtual void ShutdownModule() override;
};