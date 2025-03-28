#include "Stone.h"

void MessageQueue::check(const dpp::message_create_t& event) {
	if (event.msg.author.is_bot()) {
		return;
	}

	for (auto Obj : Message) {

		std::cout << Obj << ":" << event.msg.content << std::endl;

		if (Obj != event.msg.content) {
			continue;
		}

		(*HashSlips::HashSnowflakeStr)[RobotSlips::ObjMsg.msg.id] = std::pair<dpp::snowflake, std::string>(event.msg.id, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second);
		(*HashSlips::HashSnowflakeStr)[event.msg.id] = std::pair<dpp::snowflake, std::string>(RobotSlips::ObjMsg.msg.id, (*HashSlips::HashSnowflakeStr)[RobotSlips::ObjMsg.msg.channel_id].second);

		std::cout << "HashMessage OK~" << std::endl;
	}
}

void MessageQueue::push(std::string& message) {
	Message.push_back(message);
}

void MessageQueue::push(std::string&& message) {
	Message.push_back(std::move(message));
}


std::string markdown::MarkdownRemove(std::string str) {
	std::vector<std::tuple<std::string, std::string, std::string>> regexReplacements = {
		{ R"(\*\*([^*]+)\*\*)", "$1","**"},      // Markdown 加粗，如 **加粗** → 保留内部内容
		{ R"(\*([^*]+)\*)", "$1","*" },          // Markdown 斜体，如 *斜体* → 保留内部内容
		{ R"(__([^_]+)__)", "$1","__" },          // Markdown 下划线，如 __下划线__ → 保留内部内容
		{ R"(~~([^~]+)~~)", "$1","~~" },          // Markdown 删除线，如 ~~删除线~~ → 保留内部内容
		{ R"(\|\|([^|]+)\|\|)", "$1","||" },       // 剧透文本，如 ||剧透内容|| → 保留内部内容
		{ R"(<@!?(\d+)>)", "","" },         // 用户提及，如 <@123456789> 或 <@!987654321> → 保留数字 ID
		{ R"(<@&(\d+)>)", "","" },           // 角色提及，如 <@&111222333> → 保留数字 ID
		{ R"(<#(\d+)>)", "","" },            // 频道提及，如 <#444555666> → 保留数字 ID
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

			//附件q
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

	RobotSlips::bot->on_message_update([&](const dpp::message_update_t& event) {
		if (ChannelStone[event.msg.channel_id] == std::vector<std::pair<int, std::string
			>>() || event.msg.author.is_bot()) {
			return;
		}

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

			//附件q
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

	// 初始化 libcurl
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL* curl = curl_easy_init();
	if (curl) {
		// 设置请求头为 JSON 格式
		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type: application/json");

		// 设置请求 URL 和 POST 数据
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());

		// 执行请求
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cerr << "请求失败: " << curl_easy_strerror(res) << std::endl;
		}
		else {

		}

		// 清理资源
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}
	else {
		std::cerr << "初始化 libcurl 失败" << std::endl;
	}
	curl_global_cleanup();
}