#include <iostream>
#include <memory>
#include <stdarg.h>
#include <fstream>

#include "IBaseComponent.h"
#include "FileControll/public/FileControll.h"
#include "FileControll/public/XMLComponent.h"
#include "Internet/public/InternetComponent.h"

#include "GarbageCollector.h"



int main()
{
	setlocale(LC_ALL, "Russian");

	std::unique_ptr<XMLComponent> XML(new XMLComponent("XMLComponent"));
	std::unique_ptr<FileControllComponent> Files(new FileControllComponent("FileControll"));
	std::unique_ptr<InternetComponent> Internet(new InternetComponent("InternetComponent"));

	Internet->DownloadFile(FileControllComponent::GetRootDirectory()+"\\124.jpg", "http://almode.ru/uploads/posts/2021-04/1619739374_23-p-krasivii-dom-u-ozera-26.jpg");

	return 0;
}