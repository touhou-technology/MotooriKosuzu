#pragma once
#include <dpp/dpp.h>

#include <jsoncpp/json/json.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
/*
自定义
*/

//最后交给这个类来进行初始化（也就是app本体？）
namespace MotooriKosuzu {
	class Kosuze {
	public:
		static void Init();
		Kosuze();
		~Kosuze();
	private:
		static std::unique_ptr<Kosuze> m_Kosuze;
	};

	////该类为读取文件内容
	//class ConfigFile {
	//public:
	//	static void Init(std::string path);
	//	static Json::Value* GetJSObj();

	//	//readFile
	//	Json::Value ReadFile();

	//	ConfigFile(std::string path);
	//	~ConfigFile();

	//private:
	//	static std::unique_ptr<ConfigFile> m_ConfigFile;
	//	std::string FilePath;
	//	Json::Value JSObj;
	//};
}

