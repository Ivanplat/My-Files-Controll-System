#pragma once


#include "pch.h"

class Logger
{
public:
	static Logger* Instance();
public:
private:
	explicit Logger() noexcept;
	virtual ~Logger() noexcept;
};