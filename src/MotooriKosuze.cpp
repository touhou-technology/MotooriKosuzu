

#include "MotooriKosuze.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 

//类名就是包括该类的作用
using namespace MotooriKosuze;

//该函数为为类初始化
void Kosuze::Init() {
	m_Kosuze.reset(new Kosuze);
}

MotooriKosuze::Kosuze::Kosuze(){
	ConfigFile::Init();
	DiscordAPI::Init();
}

MotooriKosuze::Kosuze::~Kosuze(){
	m_Kosuze.reset();
}

void ConfigFile::Init() {
	m_ConfigFile.reset(new ConfigFile());
}

ConfigFile::ConfigFile() {
	ReadFile();
}

ConfigFile::~ConfigFile() {
	m_ConfigFile.reset();
}

//设置JSObj时也返回js对象
Json::Value ConfigFile::ReadFile() {
	//打开文件
	std::ifstream ifile;
	ifile.open("~/server/ConfigFile.json");

	//创建json读工厂对象
	Json::CharReaderBuilder ReaderBuilder;
	ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会编程\uxxx

	//把文件转变为json对象，要用到上面的三个变量,数据写入root
	std::string strerr;
	if (!Json::parseFromStream(ReaderBuilder, ifile, &JSObj, &strerr)) {
		std::cerr << "json解析错误";
	}

	return JSObj;
}

Json::Value* ConfigFile::GetJSObj() {
	return &(m_ConfigFile->JSObj);
}

//DiscordAPI段的实现
void DiscordAPI::Init() {
	m_DiscordAPI.reset(new DiscordAPI(ConfigFile::GetJSObj()->get("token", "null").asString()));
}

//返回一个robot以获取额，似乎可以用友元
dpp::cluster* DiscordAPI::m_robot() {
	return &bot;
}

DiscordAPI::DiscordAPI(std::string token)
	:bot(token, dpp::i_default_intents | dpp::i_message_content) {

	//启动日志
	bot.on_log(dpp::utility::cout_logger());

	//启动以线程阻塞方式
	bot.start(dpp::st_wait);
}

DiscordAPI::~DiscordAPI() {
	m_DiscordAPI.reset();
}