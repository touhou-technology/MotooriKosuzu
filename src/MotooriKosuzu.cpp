#include "MotooriKosuzu.h"

using namespace std;

Kosuzu::Kosuzu(): config_dir("../config/"), local_dir("../local/") {}

Kosuzu::~Kosuzu() {

}

void Kosuzu::Init(){
	string token = "";
	kosuzu_bot(token, dpp::i_default_intents | dpp::i_message_content /*add permission as needed*/);
	kosuzu_bot.on_log(dpp::utility::cout_logger());
}