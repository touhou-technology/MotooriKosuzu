#include "Stone.h"

StoneTranslationObj::StoneTranslationObj() {
	ChangeWrie(ConfigSlips::ConfigJson["webhook"]);
	Stone();
}

void StoneTranslationObj::ChangeWrie(nlohmann::json& tmp) {
	this->Write = tmp;
	for (auto Obj : Write["ChannelWebhook"]) {
		dpp::webhook wh(Obj);
		Channel.push_back(wh.channel_id);
	}

	for (auto debug : Channel) {
		std::cout << debug << std::endl;
	}
}

void StoneTranslationObj::Stone() {
	RobotSlips::bot->on_message_create([&](const dpp::message_create_t& event) {
		
		std::string TextMsg = event.msg.content;

		//url×ö´¦Àí
		std::vector<std::string> Treatment = StringPen::RegexTreatment(TextMsg);



		for (auto Obj : Write["ChannlKey"]) {

			if (event.msg.channel_id != Channel[Obj[0]])
				continue;



			nlohmann::json data = event.msg.to_json();
			event.msg.author.get_avatar_url();
			event.msg.author.username;
			event.msg.content;
		}
		});

}

void StoneTranslationObj::WebhookSend(std::string_view& webhook, const dpp::message_create_t& event){

}
