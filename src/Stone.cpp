#include "Stone.h"

//TODO: 重构为更稳重的逻辑
void StoneMessageDispose::check(const common_message event) {
	//发送的翻译内容
	auto& translate_msg = event.msg.content;

	for (auto iter = Obj.begin(); iter != Obj.end(); iter++) {
		//hash
		auto& [channel_id, content] = (*iter).translate_content[ChannelIndex[event.msg.channel_id] - 1];

		//debug
		std::clog << content << ":" << translate_msg << std::endl;

		if (content != translate_msg) {
			continue;
		}

		//建立hash表
		auto& [a, b] = (*iter).content_origin;
		MessageStoneHash[event.msg.id] = MessageStoneHash[a];
		MessageStoneHash[event.msg.id].get()->push_back({ event.msg.id, event.msg.channel_id });

		//debug
		std::cout << "LINK" << std::endl;

		(*iter).translate_content.erase({ (*iter).translate_content.begin() + ChannelIndex[event.msg.channel_id] });

		if ((*iter).translate_content.begin() == (*iter).translate_content.end()) {
			Obj.erase(iter);
		}

		break;
	}
}

void StoneMessageDispose::push(StoneMessage&& StoneMessage) {
	MessageStoneInstancePtr.push_back(std::make_shared<MessageStone>());
	auto& [message_id, channel] = StoneMessage.content_origin;

	MessageStoneHash[message_id] = *(MessageStoneInstancePtr.end() - 1);
	(MessageStoneInstancePtr.end() - 1)->get()->push_back({ message_id, channel });

	Obj.push_back(StoneMessage);
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
	std::thread([&]() {
		while (1) {
			std::this_thread::sleep_for(std::chrono::days(1));
			for (auto& ptr : Queue.MessageStoneInstancePtr) {
				ptr.reset();
			}
		}
		}).detach();

	std::thread([&]() {
		while (1) {
			std::this_thread::sleep_for(std::chrono::seconds(1200));
			Queue.Obj = std::vector<StoneMessage>();
		}
		}).detach();
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
		m_instance->create_message({ event });
		});

	RobotSlips::bot->on_message_update([&](const dpp::message_update_t& event) {
		m_instance->create_message({ event });
		});

	RobotSlips::bot->on_message_delete([&](const dpp::message_delete_t& event) {
		if (Queue.MessageStoneHash[event.id] == nullptr) {
			return;
		}

		for (auto& Obj : *Queue.MessageStoneHash[event.id]) {
			if (Obj.first == event.id) {
				continue;
			}

			std::cout << event.id << std::endl;

			RobotSlips::bot->message_delete(Obj.first, Obj.second);
		}

		*Queue.MessageStoneHash[event.id] = StoneMessageDispose::MessageStone();

		});

}

void StoneTranslationObj::create_message(input_message Obj) {
	common_message event = {};
	std::string message_extend = {};

	if (const auto event_obj = std::get_if<dpp::message_create_t>(&Obj)) {
		event = { event_obj->msg };
	}
	if (const auto event_obj = std::get_if<dpp::message_update_t>(&Obj)) {
		//TODO:hash消息添加
		message_extend = "\n⫸update";
		event = { event_obj->msg };
	}

	std::jthread([&] {
		Queue.check(event);
		});

	if (ChannelStone[event.msg.channel_id] == std::vector<std::pair<int, std::string>>()
		|| event.msg.author.is_bot()) {
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

	std::queue<std::future<nlohmann::json>> FutureTranslation;
	//TranslationPen async
	for (auto& Obj : ChannelStone[event.msg.channel_id]) {
		FutureTranslation.push(std::async(std::launch::async, WebPen::TranslationPen, TextMsg, Obj.second));
	}

	for (auto& Obj : ChannelStone[event.msg.channel_id]) {
		std::string unity = "";

		auto MessageObj = (FutureTranslation.front()).get()["translations"][0];
		FutureTranslation.pop();

		if (event.msg.message_reference.message_id != 0) {
			auto& ref = event.msg.message_reference;
			unity += "&>https://discord.com/channels/" + std::to_string(ref.guild_id) + "/" + std::to_string(ref.channel_id) + "/" + std::to_string(ref.message_id) + "\n";
		}

		if (MessageObj["detected_source_language"].get<std::string>() != "empty") {
			unity += TextMsgMK.MarkdownAttached(MessageObj["text"].get<std::string>());
		}

		//附件q
		for (const auto& obj : EventJson["attachments"]) {
			if (MessageObj["detected_source_language"].get<std::string>() != "empty") {
				unity += "\n";
			}
			unity += obj["url"].get<std::string>();
		}

		//url
		for (const auto& temp : Treatment) {
			if (MessageObj["detected_source_language"].get<std::string>() != "empty") {
				unity += "\n";
			}
			unity += temp;
		}

		unity += message_extend;

		std::cout << unity << std::endl;

		jsonData["content"] = unity;
		MessageTmp.translate_content.push_back({ Channel[Obj.first].second, std::move(unity) });
		UseWebhook(jsonData, Channel[Obj.first].first);
	}

	MessageTmp.content_origin = { event.msg.id, event.msg.channel_id };
	//建立链接做准备
	Queue.forward_push(std::move(MessageTmp));
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