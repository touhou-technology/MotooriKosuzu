#include "MotooriKosuzu.h"

using namespace std;

Kosuzu::Kosuzu(): config_dir("../config/"), local_dir("../local/") {}

Kosuzu::~Kosuzu() {

}

void Kosuzu::Init(){
	string token = "";
	kosuzu_bot = make_unique<dpp::cluster>(token, dpp::i_default_intents | dpp::i_message_content /*add intents as needed*/);
	kosuzu_bot->on_log(dpp::utility::cout_logger());
}

string Kosuzu::Read_token(){
	unique_ptr<Json::Value> config = ReadJson(config_dir);
}

static unique_ptr<Json::Value> ReadJson(string dir){

	return nullptr;
}