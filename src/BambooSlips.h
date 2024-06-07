#pragma once
#include <json/json.h>
#include <memory>

class ConfigSlips {
	public:
		static std::string Path_;
		static Json::Value ConfigJson;
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
		static std::string StrTranslationURL;
		static std::string Token;
		static std::string APPID;
};