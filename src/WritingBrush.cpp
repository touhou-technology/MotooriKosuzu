#include "WritingBrush.h"
#include "BambooSlips.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void ConfigPen::init(){
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
    string strerr;
    bool ok = Json::parseFromStream(ReaderBuilder, File, &root, &strerr);
    if (!ok) {
        cerr << "json解析错误" << endl;
    }

    return root;
}

Json::Value ConfigPen::GetConfigJson(){
    return ConfigSlips::ConfigJson;
}

void WebPen::init(){

}

string WebPen::TranslationPen(string TStr){
    if (!TStr.length() > 0)
        return "?";

    return string();
}
