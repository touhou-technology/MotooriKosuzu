//取自"把字刻在石头上"(笑)
#pragma once
#include <dpp/dpp.h>
#include <memory>
#include <unordered_map>

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
	nlohmann::json Write;
	std::vector<std::pair<std::string, dpp::snowflake>> Channel;

	std::unordered_map<dpp::snowflake, std::vector<std::string>> ChannelStone;
public:
	static std::unique_ptr<StoneTranslationObj> m_instance;
};
