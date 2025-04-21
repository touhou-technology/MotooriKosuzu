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

 //保存石头中翻译的或者其他消息
struct StoneMessage {
	//channel, content
	std::vector<std::tuple<dpp::snowflake, std::string>> translate_content;

	//最基础的消息
	std::tuple<dpp::snowflake, std::string> content_origin;

	//
	bool flag;
};

//TODO
class MessageQueue {
public:
	MessageQueue() = default;
	~MessageQueue() = default;

	void check(const dpp::message_create_t& event);
	void push(const StoneMessage& StoneMessage);
	void push(const StoneMessage&& StoneMessage);
private:
	friend class StoneTranslationObj;

	std::vector<StoneMessage> Obj;
	std::unordered_map<dpp::snowflake, int> ChannelIndex;
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
	std::unordered_map<dpp::snowflake, std::vector<std::pair<int, std::string>>> ChannelStone;

public:

	//instance
	static std::unique_ptr<StoneTranslationObj> m_instance;

	//message
	MessageQueue Queue;
};
