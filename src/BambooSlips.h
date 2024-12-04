#pragma once
#include <json/json.h>
#include <httplib.h>
#include <unordered_map>
#include <memory>
#include <dpp/dpp.h>

class ConfigSlips {
public:
	static std::string Path_;
	static Json::Value ConfigJson;
};

class HashSlips {
public:
	static std::unique_ptr<std::unordered_map<dpp::snowflake, std::pair<dpp::snowflake, std::string>>> HashSnowflakeStr;
	static std::unique_ptr<std::unordered_map<std::string, void(*)(dpp::slashcommand_t*)>>SlashcommandFuntion;
};

class RobotSlips {
public:
	static std::unique_ptr<dpp::cluster> bot;
	static dpp::message_create_t ObjMsg;
};

class WebSlips {
public:
	enum {
		TranslationURL = 0,
		Link
	};

	//存放数据类型
	//使用
	static std::unique_ptr<httplib::Client> Translator;
	static std::string Token;
};

class VoiceSlips {
public:
	enum {

	}Use;

};