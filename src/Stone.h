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
	void push(StoneMessage& StoneMessage);
	void push(StoneMessage&& StoneMessage);
private:
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
public:
	void ChangeWrie(nlohmann::json& tmp);

	//��Ҫ��������
	void Stone();

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
};
