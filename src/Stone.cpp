#include "Stone.h"

StoneMessageDispose::StoneMessageDispose(){

}

void StoneMessageDispose::check(const dpp::message_create_t& event) {
	//���͵ķ�������
	auto& translate_msg = event.msg.content;

	for (auto& StoneMessageObj : Obj) {
		//hash
		auto& [channel_id, content] = StoneMessageObj.translate_content[ChannelIndex[event.msg.channel_id]];

		if (content != translate_msg) {
			continue;
		}

		//����hash��


	}
}

void StoneMessageDispose::push(StoneMessage& StoneMessage) {
	Obj.push_back(StoneMessage);
}

void StoneMessageDispose::push(StoneMessage&& StoneMessage) {
	MessageStoneInstancePtr.push_back(std::make_shared<MessageStone>());

	auto& [message_id, channel] = StoneMessage.content_origin;
	MessageStoneHash[message_id] = MessageStoneInstancePtr.end()->get();

	Obj.push_back(StoneMessage);
}

std::string markdown::MarkdownRemove(std::string str) {
	std::vector<std::tuple<std::string, std::string, std::string>> regexReplacements = {
		{ R"(\*\*([^*]+)\*\*)", "$1","**"},      // Markdown �Ӵ֣��� **�Ӵ�** �� �����ڲ�����
		{ R"(\*([^*]+)\*)", "$1","*" },          // Markdown б�壬�� *б��* �� �����ڲ�����
		{ R"(__([^_]+)__)", "$1","__" },          // Markdown �»��ߣ��� __�»���__ �� �����ڲ�����
		{ R"(~~([^~]+)~~)", "$1","~~" },          // Markdown ɾ���ߣ��� ~~ɾ����~~ �� �����ڲ�����
		{ R"(\|\|([^|]+)\|\|)", "$1","||" },       // ��͸�ı����� ||��͸����|| �� �����ڲ�����
		{ R"(<@!?(\d+)>)", "","" },         // �û��ἰ���� <@123456789> �� <@!987654321> �� �������� ID
		{ R"(<@&(\d+)>)", "","" },           // ��ɫ�ἰ���� <@&111222333> �� �������� ID
		{ R"(<#(\d+)>)", "","" },            // Ƶ���ἰ���� <#444555666> �� �������� ID
	};

	std::string tmp;
	for (const auto& Obj : regexReplacements) {
		auto [regex, rp, flag] = Obj;

		tmp = str;
		std::regex pattern(regex);
		str = std::regex_replace(str, pattern, rp);
		if (tmp.size() != str.size()) {
			Flag.push_back(flag);
		}
	}
	return str;
}

std::string markdown::MarkdownAttached(std::string&& str) {
	for (const auto& Obj : Flag) {
		str = Obj + str + Obj;
		std::cout << str << std::endl;
	}
	return str;
}

StoneTranslationObj::StoneTranslationObj() {
	ChangeWrie(ConfigSlips::ConfigJson["webhook"]);
	Stone();
}

void StoneTranslationObj::ChangeWrie(nlohmann::json& tmp) {
	this->Write = tmp;

	int index = 0;
	for (auto Obj : Write["ChannelWebhook"]) {

		Channel.push_back(std::move(std::pair<std::string, dpp::snowflake>{std::string(Obj[0]), Obj[1]}));

		Queue.ChannelIndex[Obj[1]] = index;
		index++;
	}

	for (auto Obj : Write["ChannelKey"]) {
		ChannelStone[Channel[Obj[0]].second].push_back({ Obj[1] , std::string(Obj[2]) });
	}
}

void StoneTranslationObj::Stone() {
	RobotSlips::bot->on_message_create([&](const dpp::message_create_t& event) {

		if (ChannelStone[event.msg.channel_id] == std::vector<std::pair<int, std::string
			>>() || event.msg.author.is_bot()) {
			Queue.check(event);
			return;
		}
		StoneMessage MessageTmp;
		nlohmann::json EventJson = event.msg.to_json();
		nlohmann::json jsonData;

		jsonData["username"] = event.msg.author.global_name;
		jsonData["avatar_url"] = event.msg.author.get_avatar_url();

		//create temp Text url
		std::string TextMsg = event.msg.content;
		std::vector<std::string> Treatment = StringPen::RegexTreatment(TextMsg);

		//Discord
		markdown TextMsgMK;

		TextMsg = TextMsgMK.MarkdownRemove(TextMsg);
		TextMsg = StringPen::CompatibleURL(TextMsg);

		for (auto Obj : ChannelStone[event.msg.channel_id]) {
			auto MessageObj = std::move(WebPen::TranslationPen(TextMsg, Obj.second))["translations"][0];

			if (MessageObj["detected_source_language"].get<std::string>() != "empty") {

				jsonData["content"] = TextMsgMK.MarkdownAttached(MessageObj["text"].get<std::string>());
				UseWebhook(jsonData, Channel[Obj.first].first);
			}

			//����q
			for (const auto& obj : EventJson["attachments"]) {
				jsonData["content"] = obj["url"].get<std::string>();
				UseWebhook(jsonData, Channel[Obj.first].first);
			}

			//url
			for (const auto& temp : Treatment) {
				jsonData["content"] = temp;
				UseWebhook(jsonData, Channel[Obj.first].first);
			}

			MessageTmp.translate_content.push_back({ Channel[Obj.first].first, MessageObj });
		}

		MessageTmp.content_origin = { event.msg.id, event.msg.channel_id };
		//����������׼��
		Queue.push(std::move(MessageTmp));
		});

	//TODO
	RobotSlips::bot->on_message_update([&](const dpp::message_update_t& event) {

		});

	//TODO
	RobotSlips::bot->on_message_delete([&](const dpp::message_delete_t& event) {

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