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
#include <future>
#include <mutex>

#include "BambooSlips.h"
#include "WritingBrush.h"

//common_message
struct common_message {
	common_message() = default;

	common_message(dpp::message msg) :msg(msg) {

	}

	dpp::message msg = {};
	
	std::string get_message_reference_url();
};

 //保存石头中翻译的或者其他消息
struct StoneMessage {
	//channel, content
	std::vector<std::tuple<dpp::snowflake, std::string>> translate_content;

	//message_id, channel_id
	std::tuple<dpp::snowflake, dpp::snowflake> content_origin;
};

class StoneMessageDispose {
public:
	using MessageStone = std::vector<std::pair<dpp::snowflake, dpp::snowflake>>;


	StoneMessageDispose() = default;
	~StoneMessageDispose() = default;

	void check_mutex(const common_message event);

	template <typename T,typename = std::enable_if_t<std::is_same_v<T, StoneMessage>>>
	void forward_push(T&& t) {
		push(std::forward<T>(t));
	}

	std::unordered_map<dpp::snowflake, int> GetChannelIndex();
private:
	void push(StoneMessage StoneMessage);


	friend class StoneTranslationObj;

	std::vector<StoneMessage> Obj;
	std::unordered_map<dpp::snowflake, int> ChannelIndex;


	//存放
	std::vector<std::shared_ptr<MessageStone>> MessageStoneInstancePtr;

	std::unordered_map<dpp::snowflake, std::shared_ptr<MessageStone>> MessageStoneHash;

	std::mutex mtx;
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

	using input_message = std::variant<dpp::message_create_t, dpp::message_update_t>;
public:
	void ChangeWrie(nlohmann::json& tmp);

	//主要处理任务
	void Stone();


	void create_message(input_message Obj);

	void UseWebhook(nlohmann::json& jsonDate, std::string url);
private:
	nlohmann::json Write;
	//webhook, channel_id, channel_language
	std::vector<std::tuple<std::string, dpp::snowflake, std::string>> Channel;

	//第一个入口，piar<Channel的索引，翻译的语言>
	std::unordered_map<dpp::snowflake, bool> ChannelStone;

public:

	//instance
	static std::unique_ptr<StoneTranslationObj> m_instance;

	//message
	StoneMessageDispose Queue;
};