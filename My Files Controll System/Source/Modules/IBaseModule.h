#pragma once
#include <string>

class IBaseModule
{
public:
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

typedef IBaseModule Super;