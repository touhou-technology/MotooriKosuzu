//取自"把字刻在石头上"(笑)
#pragma once
#include <dpp/dpp.h>
#include <memory>
#include <unordered_map>
#include <curl/curl.h>
#include <regex>

#include "BambooSlips.h"
#include "WritingBrush.h"

//TODO
class MessageQueue {
public:
	MessageQueue() = default;
	~MessageQueue() = default;

	void check(const dpp::message_create_t& event);

	void push(std::string& message);
	void push(std::string&& message);

private:
	std::vector<std::string> Message;
};

class markdown {
public:
	markdown() = default;

	std::string MarkdownRemove(std::string str);

	std::string MarkdownAttached(std::string&& str);
private:
	std::vector<std::string> Flag;
};

class StoneTranslationObj {
public:
	StoneTranslationObj();

	~StoneTranslationObj() = default;
public:
	void ChangeWrie(nlohmann::json& tmp);

	void Stone();

	void UseWebhook(nlohmann::json& jsonDate, std::string url);
private:
	nlohmann::json Write;
	std::vector<std::pair<std::string, dpp::snowflake>> Channel;

	//第一个入口，piar<Channel的索引，翻译的语言>
	std::unordered_map<dpp::snowflake, std::vector<std::pair<int, std::string
		>>> ChannelStone;
public:
	//MessageQueue MQ;
	static std::unique_ptr<StoneTranslationObj> m_instance;
};
