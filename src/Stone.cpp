﻿#include "Stone.h"

std::string common_message::get_message_reference_url() {
	return { "https://discord.com/channels/" + std::to_string(this->msg.guild_id) + "/" + std::to_string(this->msg.channel_id) + "/" + std::to_string(this->msg.message_reference.message_id) };
}


//TODO: 重构为更稳重的逻辑
void StoneMessageDispose::check_mutex(const common_message event) {
	std::lock_guard<std::mutex> lock(mtx);

	static int i = 1;
	if (i == ChannelIndex.size()) {
		std::cout << "del" << std::endl;

		Obj.pop_back();
		i = 1;
	}

	std::hash<std::string> translate_event_hash;
	size_t translate_hash_value = translate_event_hash(event.msg.content);

	for (auto iter = Obj.begin(); iter != Obj.end(); iter++) {
		auto& [channel_id, content] = iter->translate_content[ChannelIndex[event.msg.channel_id]];

		if (content != translate_hash_value) {
			continue;
		}

		std::cout << event.msg.content << ">LINK<" << Obj.size() << std::endl;

		auto& [message_id_origin, channel_id_origin] = iter->content_origin;
		MessageStoneHash[event.msg.id] = MessageStoneHash[message_id_origin];
		MessageStoneHash[event.msg.id].get()->at(ChannelIndex[event.msg.channel_id]) = { event.msg.id, event.msg.channel_id };

		i++;
		break;
	}
}

std::unordered_map<dpp::snowflake, int> StoneMessageDispose::GetChannelIndex() {
	return ChannelIndex;
}

void StoneMessageDispose::push(StoneMessage StoneMessage) {
	MessageStoneInstancePtr.push_back(std::make_shared<MessageStone>());
	auto& [message_id, channel] = StoneMessage.content_origin;

	MessageStoneHash[message_id] = *(MessageStoneInstancePtr.end() - 1);

	//MessageStone排序 
	for (int i = 0; i < ChannelIndex.size(); i++) {
		(MessageStoneInstancePtr.end() - 1)->get()->push_back({});
	}

	(MessageStoneInstancePtr.end() - 1)->get()->at(ChannelIndex[channel]) = { message_id, channel };

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

		Channel.push_back({ Obj[0], Obj[1], Obj[2] });

		Queue.ChannelIndex[Obj[1]] = index;
		index++;
	}

	for (auto& [webhook, channel_id, channel_language] : Channel) {
		for (auto& [webhook_i, channel_id_i, channel_language_i] : Channel) {
			if (channel_id == channel_id_i) {
				continue;
			}

			ChannelStone[channel_id] = true;
		}
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
		m_instance->del_msg({ event });
		});

}

void StoneTranslationObj::del_msg(dpp::message_delete_t event){
	std::lock_guard<std::mutex> lock(del);

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

}

void StoneTranslationObj::create_message(input_message Obj) {
	common_message event = {};
	std::string message_extend = {};

	if (const auto event_obj = std::get_if<dpp::message_create_t>(&Obj)) {
		event = { event_obj->msg };
	}

	if (const auto event_obj = std::get_if<dpp::message_update_t>(&Obj)) {
		event = { event_obj->msg };

		dpp::message_delete_t DelObj;
		DelObj.id = event.msg.id;
		DelObj.channel_id = event.msg.channel_id;
		DelObj.guild_id = event.msg.guild_id;

		m_instance->del_msg({ DelObj });

		message_extend = "\n<update>";
	}

	//Message link
	std::thread([&] {
		Queue.check_mutex(event);
		}).detach();

	if (ChannelStone[event.msg.channel_id] != true
		|| event.msg.author.is_bot()) {
		return;
	}

	StoneMessage MessageTmp;
	MessageTmp.translate_content.reserve(Queue.GetChannelIndex().at(event.msg.channel_id));

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
	//TranslationPen async push
	for (auto& [webhook, channel_id, channel_language] : Channel) {
		if (channel_id == event.msg.channel_id) {
			continue;
		}

		FutureTranslation.push(std::async(std::launch::async, WebPen::TranslationPen, TextMsg, channel_language));
	}

	std::hash<std::string> hasher;
	//TranslationPen
	for (auto& [webhook, channel_id, channel_language] : Channel) {

		if (channel_id == event.msg.channel_id) {
			MessageTmp.translate_content.push_back({});
			continue;
		}

		std::string unity = "";

		auto MessageObj = std::move((FutureTranslation.front().get())["translations"][0]);
		FutureTranslation.pop();

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

		//reference
		if (event.msg.message_reference.message_id != dpp::snowflake{}) {
			std::string tmp = {};

			//TODO
			if (Queue.MessageStoneHash[event.msg.message_reference.message_id] != nullptr) {
				for (auto& [message_id_i, channel_id_i] : *Queue.MessageStoneHash[event.msg.message_reference.message_id]) {
					if (channel_id == channel_id_i) {
						tmp = "&>[☯](https://discord.com/channels/" + std::to_string(event.msg.guild_id) + "/" + std::to_string(channel_id_i) + "/" + std::to_string(message_id_i) + ")\n";

						break;
					}
				}
			}

			if (tmp == "") {
				unity = "&>[☯](" + event.get_message_reference_url() + ")\n" + unity;
			}
			else {
				unity = tmp + unity;
			}
		}

		unity += message_extend;

		//debug
		std::cout << unity << std::endl;

		jsonData["content"] = unity;
		std::thread([&] {UseWebhook(jsonData, webhook); }).detach();

		size_t hash_value = hasher(unity);

		MessageTmp.translate_content.push_back({ channel_id, std::move(hash_value) });
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

