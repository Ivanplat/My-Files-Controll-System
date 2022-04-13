#include <iostream>
#include <memory>
#include <stdarg.h>


#include "IBaseComponent.h"
#include "FileControll/public/FileControll.h"



int main()
{
	std::unique_ptr<FileControllComponent> Test(new FileControllComponent("FileControll"));
	Test->StartupModule();
	//Test.release();
	while (true);

	return 0;
}