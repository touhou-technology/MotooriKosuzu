//取自"把字刻在石头上"(笑)
#pragma once
#include <dpp/dpp.h>
#include <memory>
#include <unordered_map>
#include <curl/curl.h>

#include "BambooSlips.h"
#include "WritingBrush.h"

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
	static std::unique_ptr<StoneTranslationObj> m_instance;
};

class markdown {
public:
	markdown() = default;

	std::string MarkdownRemove(std::string&& str);

	std::string MarkdownAttached(std::string&& str);
private:
	
};