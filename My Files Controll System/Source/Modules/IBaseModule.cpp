#include "pch.h"
#include "IBaseModule.h"

IBaseModule::IBaseModule(std::string ComponentName) noexcept : ComponentName(ComponentName)
{

}

IBaseModule::~IBaseModule() noexcept
{
}
