#pragma once

#include "Modules/IBaseModule.h"

#include <chrono>
#include <sstream>


struct FDate
{
	std::chrono::days days;
	std::chrono::months months;
	std::chrono::years years;

	FDate(std::chrono::days days, std::chrono::months months, std::chrono::years years) :
		days(days), months(months), years(years) {}
	FDate()
	{
		std::time_t time = std::time(0);
		std::tm now;
		localtime_s(&now, &time);
		days = std::chrono::days(now.tm_mday);
		months = std::chrono::months(now.tm_mon);
		years = std::chrono::years(1900) + std::chrono::years(now.tm_year);
	}
	std::string ToString()
	{
		std::stringstream ss;
		ss << std::to_string(days.count()) + "-" + std::to_string(months.count()) + "-" + std::to_string(years.count());
		return ss.str();
	}
};

struct FTime
{
	std::chrono::seconds seconds;
	std::chrono::minutes minutes;
	std::chrono::hours hours;

	FTime(std::chrono::seconds seconds, std::chrono::minutes minutes, std::chrono::hours hours) :
		seconds(seconds), minutes(minutes), hours(hours) {}
	FTime()
	{ 
		std::time_t time = std::time(0);
		std::tm now;
		localtime_s(&now, &time);
		seconds = std::chrono::seconds(now.tm_sec);
		minutes = std::chrono::minutes(now.tm_min);
		hours = std::chrono::hours(now.tm_hour);
	}
	std::string ToString()
	{
		std::stringstream ss;
		ss << std::to_string((hours.count())) + "-" + std::to_string(minutes.count()) + "-"  + std::to_string(seconds.count());
		return ss.str();
	}
};

struct FDateTime : public virtual FDate, public virtual FTime
{
	FDateTime(std::chrono::days days, std::chrono::months mounths, std::chrono::years years,
		std::chrono::seconds seconds, std::chrono::minutes minutes, std::chrono::hours hours) : FDate(days, mounths, years), FTime(seconds, minutes, hours) {}
	FDateTime() : FDate(), FTime() {}

	std::string ToString()
	{
		return FTime::ToString() + '-' + FDate::ToString();
	}
};

class SystemModule: public virtual IBaseModule
{
public:
	explicit SystemModule(std::string ComponentName) noexcept : Super(ComponentName) {}
public:
	virtual void StartupModule() override;
	virtual void Update() override;
public:
	FDateTime GetCurrentTime();
};