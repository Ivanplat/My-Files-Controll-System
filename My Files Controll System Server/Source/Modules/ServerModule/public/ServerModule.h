#pragma once
#include "Modules/IBaseModule.h"


class ServerModule : public virtual IBaseModule
{
public:
	explicit ServerModule(std::string ModuleName) noexcept : Super(ModuleName) {};
public:
	virtual void StartupModule() override;
	virtual void Update() override;
	virtual void ShutdownModule() override;
};
