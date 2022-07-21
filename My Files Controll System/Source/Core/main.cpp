#include "pch.h"
#include "Modules/FilesControlModule/FilesControlModule.h"
#include "Modules/SystemModule/SystemModule.h"
#include "Modules/XMLModule/XMLModule.h"
#include "Modules/CommandParserModule/CommandParserModule.h"

int main()
{
	try
	{
		//GC->System->DeleteProject("C:\\Users\\EventGraph\\Desktop\\Project1");
		//GC->System->InitializeNewProject("C:\\Users\\EventGraph\\Desktop\\Project1");
		//GC->XML->AddIgnoreFile("C:\\Users\\EventGraph\\Desktop\\Test\\123.txt");
		//GC->XML->InitializeNewProject();
		//GC->FilesControl->InitializeProject();
		//GC->System->SelectProject("C:\\Users\\EventGraph\\Desktop\\Project1");
		while (true)
		{
			std::cout << ">$ ";
			std::string command;
			std::getline(std::cin, command);	
			GC->CommandParser->ParseConsoleCommand(command);
		}
	}
	catch (const FilesControlModuleExeption& filesControlModuleException)
	{
		std::cerr << filesControlModuleException.what() << "\n";
		main();
	}
	catch (const XMLModuleExeption& xmlModuleException)
	{
		std::cerr << xmlModuleException.what() << "\n";
		main();
	}
	catch (const ProjectInitializeExeption& projectInitializeException)
	{
		std::cerr << projectInitializeException.what() << "\n";
		main();
	}
	catch (const SystemModuleException& systemModuleException)
	{
		std::cerr << systemModuleException.what() << "\n";
		main();
	}
	catch (const CommandExeption& commandExeption)
	{
		std::cerr << commandExeption.what() << "\n";
		main();
	}
	catch (const ModuleException& moduleException)
	{
		std::cerr << moduleException.what() << "\n";
		main();
	}
	catch (const std::filesystem::filesystem_error& filesystemException)
	{
		std::cerr << filesystemException.what() << "\n";
		main();
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what() << "\n";
		main();
	}
	return 0;
}