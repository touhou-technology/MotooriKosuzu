//取自"把字刻在石头上"(笑)
#pragma once
#include <dpp/dpp.h>

#include "BambooSlips.h"
#include "WritingBrush.h"

class StoneTranslationObj {
public:
	StoneTranslationObj();
	
	~StoneTranslationObj() = default;
public:
	void ChangeWrie(nlohmann::json& tmp);
	
	void Stone();
private:
	void WebhookSend(std::string_view& webhook,const dpp::message_create_t& event);

	nlohmann::json Write;
	std::vector<dpp::snowflake> Channel;
};

