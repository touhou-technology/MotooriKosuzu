#ifndef STONE_H
#define STONE_H

#include <curl/curl.h>
#include <dpp/dpp.h>
#include <memory>

#include <future>
#include <mutex>
#include <regex>
#include <thread>
#include <unordered_map>

#include "BambooSlips.h"
#include "WritingBrush.h"

// common_message
struct common_message {
	common_message() = default;

	common_message(dpp::message msg) : msg(msg) {}

	common_message(dpp::message::message_ref msg_ref) {
		msg.id = msg_ref.message_id;
		msg.channel_id = msg_ref.channel_id;
		msg.guild_id = msg_ref.guild_id;
	}

	dpp::message msg = {};

	std::string get_message_reference_url();
};

struct StoneMessage {
	// channel, content
	std::vector<std::tuple<dpp::snowflake, size_t>> translate_content;

	// message_id, channel_id
	std::tuple<dpp::snowflake, dpp::snowflake> content_origin;

	int flag = 0;
};

class StoneMessageDispose {
  public:
	using MessageStone = std::vector<std::pair<dpp::snowflake, dpp::snowflake>>;

	StoneMessageDispose() = default;
	~StoneMessageDispose() = default;

	void check_mutex(const common_message event);

	template <typename T,
			  typename = std::enable_if_t<std::is_same_v<T, StoneMessage>>>
	void forward_push(T &&t) {
		push(std::forward<T>(t));
	}

	std::unordered_map<dpp::snowflake, int> GetChannelIndex();

  private:
	void push(StoneMessage StoneMessage);

	friend class StoneTranslationObj;

	std::vector<StoneMessage> Obj;
	std::unordered_map<dpp::snowflake, int> ChannelIndex;

	std::vector<std::shared_ptr<MessageStone>> MessageStoneInstancePtr;

	std::unordered_map<dpp::snowflake, std::shared_ptr<MessageStone>>
		MessageStoneHash;

	std::mutex mtx;
};

class markdown {
  public:
	markdown() = default;

	std::string MarkdownRemove(std::string str);

	std::string MarkdownAttached(std::string &&str);

  private:
	std::vector<std::string> Flag;
};

class StoneTranslationObj {
  public:
	StoneTranslationObj();

	~StoneTranslationObj() = default;

	using input_message =
		std::variant<dpp::message_create_t, dpp::message_update_t>;

  public:
	void ChangeWrie(nlohmann::json &tmp);

	void Stone();

	void del_msg(dpp::message_delete_t event);

	void create_message(input_message Obj);

	void UseWebhook(nlohmann::json &jsonDate, std::string url);

  private:
	std::mutex del;

	nlohmann::json Write;
	// webhook, channel_id, channel_language
	std::vector<std::tuple<std::string, dpp::snowflake, std::string>> Channel;

	std::unordered_map<dpp::snowflake, bool> ChannelStone;

  public:
	// instance
	static std::unique_ptr<StoneTranslationObj> m_instance;

	// message
	StoneMessageDispose Queue;
};
#endif /* STONE_H */
