#pragma once

#include <memory>
#include <cassert>
#include <vector>

#include "Core/IBaseComponent.h"

class GarbageCollector
{
public:
	static GarbageCollector* Instance();
public:
	template<class T>
	std::unique_ptr<T> CreateNewComponent(std::string ComponentName);
private:
	std::vector<std::unique_ptr<IBaseComponent>> Components;
private:
	explicit GarbageCollector() noexcept = default;
	virtual ~GarbageCollector() noexcept = default;
};

template<class T>
inline std::unique_ptr<T> GarbageCollector::CreateNewComponent(std::string ComponentName)
{
	auto n = std::unique_ptr<T>(new T(ComponentName));
	Components.push_back(dynamic_cast<std::unique_ptr<IBaseComponent>>(n));
	return n;
}
