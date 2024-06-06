#include "WritingBrush.h"
#include "BambooSlips.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void ConfigPen::Init() {
	ConfigSlips::ConfigJson = ReadFileJson(ConfigSlips::Path_);
}

Json::Value ConfigPen::ReadFileJson(string Path) {
	ifstream File(Path);

	if (!File.is_open()) {
		cerr << "cennt open file";
	}

	Json::CharReaderBuilder ReaderBuilder;
	ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会编程\uxxx

	Json::Value root;

	//把文件转变为json对象
	std::string strerr;
	bool ok = Json::parseFromStream(ReaderBuilder, File, &root, &strerr);
	if (!ok) {
		std::cerr << "json解析错误" << std::endl;
	}

	return root;
}

Json::Value ConfigPen::GetConfigJson() {
	return ConfigSlips::ConfigJson;
}

void RobotPen::Init() {
	RobotSlips::bot.reset(new dpp::cluster(ConfigPen::GetConfigJson()["BotToken"].asString(), dpp::i_default_intents | dpp::i_message_content));
}

void RobotPen::work(void(*Fn)(dpp::cluster* bot)) {
	Fn(&*RobotSlips::bot);
}
//Adapt to Lambda
dpp::cluster* RobotPen::GetBot(){
	return &*RobotSlips::bot;
}

void WebPen::Init() {

}

std::string WebPen::TranslationPen(std::string TStr) {
	return std::string();
}