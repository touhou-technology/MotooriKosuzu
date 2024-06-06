#include "MotooriKosuzu.h"

using namespace std;

Kosuzu::Kosuzu(): config_dir("../config/"), local_dir("../local/") {}

Kosuzu::~Kosuzu() {}

void Kosuzu::Init(){
	Read_config();
	string token = config->get("token", "null").asString();
	kosuzu_bot = make_unique<dpp::cluster>(token, dpp::i_default_intents | dpp::i_message_content /*add intents as needed*/);
	kosuzu_bot->on_log(dpp::utility::cout_logger());
}

void Kosuzu::Reset(){
	Init();
}

void Kosuzu::Read_config(){
	config = ReadJson(config_dir);
}

unique_ptr<Json::Value> Kosuzu::ReadJson(string dir){
	ifstream file(dir);

	if (!file.is_open()) {
		cerr << "cennt open file";
	}

	Json::CharReaderBuilder ReaderBuilder;
	ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会编程\uxxx

	unique_ptr<Json::Value> root;

	//把文件转变为json对象
	string strerr;

	if (!Json::parseFromStream(ReaderBuilder, file, root.get(), &strerr)) {
		cerr << "json解析错误" << endl;
	}

	return root;
}