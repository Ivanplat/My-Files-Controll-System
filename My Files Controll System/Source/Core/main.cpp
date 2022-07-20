#include "pch.h"
#include "Modules/FilesControlModule/FilesControlModule.h"
#include "Modules/SystemModule/SystemModule.h"
#include "Modules/XMLModule/XMLModule.h"

int main()
{
	try
	{
		GC->System->DeleteProject("C:\\Users\\EventGraph\\Desktop\\Project1");
		GC->System->InitializeNewProject("C:\\Users\\EventGraph\\Desktop\\Project1");
		//GC->XML->AddIgnoreFile("C:\\Users\\EventGraph\\Desktop\\Test\\123.txt");
		//GC->XML->InitializeNewProject();
		//GC->FilesControl->InitializeProject();
	}
	catch (const FilesControlModuleExeption& filesControlModuleException)
	{
		std::cerr << filesControlModuleException.what() << "\n";
	}
	catch (const XMLModuleExeption& xmlModuleException)
	{
		std::cerr << xmlModuleException.what() << "\n";
	}
	catch (const ProjectInitializeExeption& projectInitializeException)
	{
		std::cerr << projectInitializeException.what() << "\n";
	}
	catch (const SystemModuleException& systemModuleException)
	{
		std::cerr << systemModuleException.what() << "\n";
	}
	catch (const ModuleException& moduleException)
	{
		std::cerr << moduleException.what() << "\n";
	}
	return 0;
}