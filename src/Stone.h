/**取自"把字刻在石头上"(笑)
 * 对channel中翻译
 * 按照配置文件来进行
 */
#pragma once
#include <dpp/dpp.h>
#include <curl/curl.h>
#include <memory>

#include <unordered_map>
#include <regex>
#include <thread>
#include <atomic>

#include "BambooSlips.h"
#include "WritingBrush.h"

class base {

};

//TODO
class MessageQueue {
public:
	MessageQueue() = default;
	~MessageQueue() = default;

	void check(const dpp::message_create_t& event);
	void push(const base& base);
	void push(const base&& base);
private:
	std::vector<base> Obj;
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

	//主要处理任务
	void Stone();

	void UseWebhook(nlohmann::json& jsonDate, std::string url);
private:
	nlohmann::json Write;
	std::vector<std::pair<std::string, dpp::snowflake>> Channel;

	//第一个入口，piar<Channel的索引，翻译的语言>
	std::unordered_map<dpp::snowflake, std::vector<std::pair<int, std::string
		>>> ChannelStone;
public:

	//instance
	static std::unique_ptr<StoneTranslationObj> m_instance;

	//message
	MessageQueue Queue;
};
