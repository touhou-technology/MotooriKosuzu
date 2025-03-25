//ȡ��"���ֿ���ʯͷ��"(Ц)
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

	//��һ����ڣ�piar<Channel�����������������>
	std::unordered_map<dpp::snowflake, std::vector<std::pair<int, std::string
		>>> ChannelStone;
public:
	static std::unique_ptr<StoneTranslationObj> m_instance;
	MessageQueue c_MessageQueue;
};

class MessageQueue {
public:
	MessageQueue();
	~MessageQueue() = default;

	void push(std::string& message);
	void push(std::string&& message);

private:
	std::vector<std::string> Message;
};