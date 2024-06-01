#include "MotooriKosuzu.h"

//类名就是包括该类的作用
using namespace MotooriKosuzu;


//该函数为为类初始化
void Kosuze::Init() {
	m_Kosuze.reset(new Kosuze());
}

Kosuze::Kosuze() {
	std::cout << "TEST";
}

Kosuze::~Kosuze() {

}

//void ConfigFile::Init(std::string path) {
//	m_ConfigFile.reset(new ConfigFile(path));
//}
//
//Json::Value* MoKu::ConfigFile::GetJSObj() {
//	return &(m_ConfigFile->JSObj);
//}
//
//Json::Value MoKu::ConfigFile::ReadFile() {
//	return Json::Value();
//}
//
//MoKu::ConfigFile::ConfigFile(std::string path)
//	:FilePath(path), JSObj(ReadFile()) {
//	std::cout << "TEST-2";
//}
//
//MoKu::ConfigFile::~ConfigFile() {
//
//}
