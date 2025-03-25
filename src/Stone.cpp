#include "Stone.h"

StoneTranslationObj::StoneTranslationObj() {
	ChangeWrie(ConfigSlips::ConfigJson["webhook"]);
	Stone();
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

		if (ChannelStone[event.msg.channel_id] == std::vector<std::pair<int, std::string
			>>() || event.msg.author.is_bot()) {
			return;
		}

		nlohmann::json EventJson = event.msg.to_json();
		nlohmann::json jsonData;

		jsonData["username"] = event.msg.author.global_name;
		jsonData["avatar_url"] = event.msg.author.get_avatar_url();

		dpp::snowflake re = NULL;

		//create temp Text url
		std::string TextMsg = event.msg.content;
		std::vector<std::string> Treatment = StringPen::RegexTreatment(TextMsg);

		markdown TextMsgMK;

		TextMsg = TextMsgMK.MarkdownRemove(std::move(TextMsg));

		std::cout << TextMsg << std::endl;

		TextMsg = StringPen::CompatibleURL(TextMsg);

		for (auto Obj : ChannelStone[event.msg.channel_id]) {
			auto MessageObj = std::move(WebPen::TranslationPen(TextMsg, Obj.second))["translations"][0];


			if (MessageObj["detected_source_language"].get<std::string>() != "empty") {

				TextMsgMK.MarkdownAttached(MessageObj["text"].get<std::string>());

				jsonData["content"] = MessageObj["text"].get<std::string>();
				UseWebhook(jsonData, Channel[Obj.first].first);
			}


			//����
			for (const auto& obj : EventJson["attachments"]) {
				jsonData["content"] = obj["url"].get<std::string>();
				UseWebhook(jsonData, Channel[Obj.first].first);
			}

			//url
			for (const auto& temp : Treatment) {
				jsonData["content"] = temp;
				UseWebhook(jsonData, Channel[Obj.first].first);
			}
		}

		});

}

void StoneTranslationObj::UseWebhook(nlohmann::json& jsonData, std::string url) {
	std::string jsonStr = jsonData.dump();

	// ��ʼ�� libcurl
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL* curl = curl_easy_init();
	if (curl) {
		// ��������ͷΪ JSON ��ʽ
		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type: application/json");

		// �������� URL �� POST ����
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());

		// ִ������
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cerr << "����ʧ��: " << curl_easy_strerror(res) << std::endl;
		}
		else {

		}

		// ������Դ
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}
	else {
		std::cerr << "��ʼ�� libcurl ʧ��" << std::endl;
	}
	curl_global_cleanup();
}

std::string markdown::MarkdownRemove(std::string&& str){

	return str;
}

std::string markdown::MarkdownAttached(std::string&& str){
	return str;
}