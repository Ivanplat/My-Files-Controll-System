#pragma once
#include <string>

class IBaseComponent
{
public:
	explicit IBaseComponent(std::string ComponentName) noexcept;
	virtual ~IBaseComponent() noexcept;
public:
	std::string GetName() { return ComponentName;  };
	virtual void StartupModule() = 0;
	virtual void Update() = 0;
private:
protected:
	std::string ComponentName;
};

typedef IBaseComponent Super;