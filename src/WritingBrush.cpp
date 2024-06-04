#include "WritingBrush.h"
#include "BambooSlips.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace Pen;

void ConfigPen::init(){
    BambooSlips::ConfigSlips::
}

Json::Value ConfigPen::ReadFile(std::string Path = BambooSlips::ConfigSlips::Path_) {
	std::ifstream File(Path);

	if (!File.is_open()) {
		std::cerr << "cennt open file";
	}

    Json::CharReaderBuilder ReaderBuilder;
    ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会编程\uxxx

    Json::Value root;

    //把文件转变为json对象
    std::string strerr;
    bool ok = Json::parseFromStream(ReaderBuilder, File, &root, &strerr);
    if (!ok) {
        std::cerr << "json解析错误";
    }

    return root;
}

void WebPen::init(){

}

std::string WebPen::TranslationPen(std::string TStr){
    if (!TStr.length() > 0)
        return "?";

    std::string temHTTPURL = BambooSlips::WebSlips::GetTranslationURL();

    return std::string();
}
