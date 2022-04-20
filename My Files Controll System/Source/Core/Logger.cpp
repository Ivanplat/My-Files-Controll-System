#include "pch.h"


Logger* Logger::Instance()
{
	static Logger lg;
	return &lg;
}

void Logger::PrintToLog(std::string LogString)
{
	auto Time = GC->System->GetCurrentTime().ToString();
	std::ofstream out(CurrentLogFilePath.string(), std::ios::app);
	out << LogString << " : " << Time << std::endl;
	out.close();
}

void Logger::CreateLogFile()
{
	auto Time = GC->System->GetCurrentTime().ToString();
	auto logPath = GC->Files->GetRootDirectory().string() + "\\FilesControll\\log\\log_" + Time;
	CurrentLogFilePath = logPath;
	std::ofstream out(logPath, std::ios::app);
	out << "Monolit file controll log" << std::endl;
	out << Time << std::endl;
	out.close();
}
