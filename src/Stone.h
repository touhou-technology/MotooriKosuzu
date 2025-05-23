/**ȡ��"���ֿ���ʯͷ��"(Ц)
 * ��channel�з���
 * ���������ļ�������
 */
#pragma once
#include <dpp/dpp.h>
#include <curl/curl.h>
#include <memory>

#include <unordered_map>
#include <regex>
#include <thread>
#include <future>

#include "BambooSlips.h"
#include "WritingBrush.h"

 //����ʯͷ�з���Ļ���������Ϣ
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

	void check(const dpp::message_create_t& event);
	void check(const dpp::message_update_t& event);

	template <typename T,typename = std::enable_if_t<std::is_same_v<T, StoneMessage>>>
	void forward_push(T&& t) {
		push(std::forward<T>(t));
	}

private:
	void push(StoneMessage&& StoneMessage);


	friend class StoneTranslationObj;

	std::vector<StoneMessage> Obj;
	std::unordered_map<dpp::snowflake, int> ChannelIndex;


	//���w
	std::vector<std::shared_ptr<MessageStone>> MessageStoneInstancePtr;

	std::unordered_map<dpp::snowflake, std::shared_ptr<MessageStone>> MessageStoneHash;
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

	//��Ҫ��������
	void Stone();


	void create_message(input_message Obj);

	void UseWebhook(nlohmann::json& jsonDate, std::string url);
private:
	nlohmann::json Write;
	std::vector<std::pair<std::string, dpp::snowflake>> Channel;

	//��һ����ڣ�piar<Channel�����������������>
	std::unordered_map<dpp::snowflake, std::vector<std::pair<int, std::string>>> ChannelStone;

public:

	//instance
	static std::unique_ptr<StoneTranslationObj> m_instance;

	//message
	StoneMessageDispose Queue;

	struct message {
		message() = default;

		message(dpp::message msg) :msg(msg) {

		}

		dpp::message msg = {};
	};
};