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

	for (auto Obj : Write["ChannelKey"]) {
		ChannelStone[Channel[Obj[0]].second].push_back({ Obj[1] , std::string(Obj[2]) });
	}
}

void StoneTranslationObj::Stone() {
	RobotSlips::bot->on_message_create([&](const dpp::message_create_t& event) {

		auto it = ChannelStone.find(event.msg.channel_id);
		if (it != ChannelStone.end() && !it->second.empty()) {
			return;
		}

		for (auto Obj : ChannelStone[event.msg.channel_id]) {






		}







		std::string TextMsg = event.msg.content;

		//url������
		std::vector<std::string> Treatment = StringPen::RegexTreatment(TextMsg);


		for (auto Key : Write["ChannlKey"]) {



			nlohmann::json data = event.msg.to_json();
			event.msg.author.get_avatar_url();
			event.msg.author.username;
			event.msg.content;
		}
		});

}

void StoneTranslationObj::UseWebhook(nlohmann::json& jsonDate, std::string_view& url, std::string_view content){

}