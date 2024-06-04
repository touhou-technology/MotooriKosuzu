#pragma once
#include <iostream>

namespace BambooSlips {
	class ConfigSlips {
	public:
		static std::string Path_;
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