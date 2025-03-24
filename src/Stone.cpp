#include "Stone.h"

StoneTranslationObj::StoneTranslationObj() {
	ChangeWrie(ConfigSlips::ConfigJson["webhook"]);
	//Stone();
}

void StoneTranslationObj::ChangeWrie(nlohmann::json& tmp) {
	this->Write = tmp;

	for (auto Obj : Write["ChannelWebhook"]) {

		Channel.push_back(std::move(std::pair<std::string, dpp::snowflake>{std::string(Obj[0]), Obj[1]}));
	}

	for (auto Obj: Write["ChannelKey"]) {
	
	}
}

void StoneTranslationObj::Stone() {
	RobotSlips::bot->on_message_create([&](const dpp::message_create_t& event) {



		std::string TextMsg = event.msg.content;

		//url×ö´¦Àí
		std::vector<std::string> Treatment = StringPen::RegexTreatment(TextMsg);


		for (auto Key : Write["ChannlKey"]) {



			nlohmann::json data = event.msg.to_json();
			event.msg.author.get_avatar_url();
			event.msg.author.username;
			event.msg.content;
		}
		});

}