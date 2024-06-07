#pragma once
#include <json/json.h>
#include <httplib.h>
#include <map>
#include <memory>
#include <dpp/dpp.h>

class ConfigSlips {
public:
	static std::string Path_;
	static Json::Value ConfigJson;
};

class HashSlips {
public:
	static std::map<dpp::snowflake, dpp::snowflake> MessageHash;
};

class RobotSlips {
public:
	static std::unique_ptr<dpp::cluster> bot;
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
	static std::string StrTranslationURL;
	static std::string Token;
	static std::string APPID;
};