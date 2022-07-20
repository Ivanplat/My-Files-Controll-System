#pragma once
#include <string>
#include <vcruntime_typeinfo.h>
#include <exception>

class ModuleException : public std::exception 
{
public:
	explicit ModuleException(const char* _Message) : std::exception(_Message) {}
};

class IBaseModule
{
protected:
	typedef IBaseModule Super;
public:
	explicit IBaseModule() noexcept { ComponentName = typeid(*this).name(); }
	explicit IBaseModule(std::string ComponentName) noexcept;
	virtual ~IBaseModule() noexcept;
public:
	std::string GetName() { return ComponentName;  };
	virtual void StartupModule() = 0;
	virtual void ShutdownModule() = 0;
	virtual void Update() = 0;
private:
protected:
	std::string ComponentName;
};