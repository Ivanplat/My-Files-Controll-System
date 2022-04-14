#include <iostream>
#include <memory>
#include <stdarg.h>
#include <fstream>

#include "IBaseComponent.h"
#include "FileControll/public/FileControll.h"
#include "FileControll/public/XMLComponent.h"



int main()
{
	/*std::unique_ptr<FileControllComponent> Test(new FileControllComponent("FileControll"));
	Test->StartupModule();*/
	//Test.release();
	setlocale(LC_ALL, "Russian");

	//std::ifstream f(std::filesystem::current_path().string() + "\\" + "FilesControll\\checkout.cfg");
	//std::string buf;
	//std::vector<std::string> vbuf;
	//while (std::getline(f, buf))
	//{
	//	vbuf.push_back(buf);
	//}
	//for (auto& i : vbuf)
	//{
	//	std::cout << i << std::endl;
	//}

	XMLComponent xml("XMLComponent");
	xml.StartupModule();
	xml.LoadIgnoreDocument();
	//xml.RemoveDirectoryFromIgnore("C:\\Users\\EventGraph\\source\\repos\\My Files Controll System\\My Files Controll System\\Intermidate");
	/*xml.AddDirectoryToIgnore("C:\\Users\\EventGraph\\source\\repos\\My Files Controll System\\My Files Controll System\\Intermidate");
	xml.AddDirectoryToIgnore("C:\\Users\\EventGraph\\source\\repos\\My Files Controll System\\My Files Controll System\\Intermidate");
	xml.AddFileToIgnore("C:\\Users\\EventGraph\\source\\repos\\My Files Controll System\\My Files Controll System\\Intermidate\\123.txt", "1231231");
	xml.AddFileToIgnore("C:\\Users\\EventGraph\\source\\repos\\My Files Controll System\\My Files Controll System\\Intermidate\\1dfgd3.txt", "1231231");
	xml.AddFileToIgnore("C:\\Users\\EventGraph\\source\\repos\\My Files Controll System\\My Files Controll System\\Intermidate\\1223.txt", "1231231");
	xml.AddFileToIgnore("C:\\Users\\EventGraph\\source\\repos\\My Files Controll System\\My Files Controll System\\Intermidate\\124.txt", "1231231");*/
	xml.SaveIgnoreDocument();
	//xml.RemoveFileFromIgnore("C:\\Users\\EventGraph\\source\\repos\\My Files Controll System\\My Files Controll System\\Intermidate\\123.txt");
	//while (true);

	return 0;
}