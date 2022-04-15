#pragma once

#include <string>

class IBaseModule
{
public:
	explicit IBaseModule(std::string ModuleName) noexcept : ModuleName(ModuleName) {};
	virtual ~IBaseModule() noexcept {};
public:
	virtual void StartupModule() = 0;
	virtual void Update() = 0;
	virtual void ShutdownModule() = 0;
protected:
	std::string ModuleName;
};

typedef IBaseModule Super;