#pragma once
#include <json/json.h>
#include <memory>

namespace BambooSlips {

	class ConfigSlips {
	public:
		static std::string Path_;
		static Json::Value ConfigJson;
	};

	class WebSlips {
	public:
		enum {
			TranslationURL = 0,
			Link
		};
		//存放数据类型
		static std::string StrTranslationURL;
	};
}