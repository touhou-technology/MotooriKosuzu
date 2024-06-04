#pragma once
#include <iostream>

namespace BambooSlips {
	class ConfigSlips {
	public:
		static std::string Path_;
	};
	class WebSlips {
	public:
		static std::string GetTranslationURL();
		enum {
			TranslationURL = 0,
			Link
		};
		static void SetURL(std::string Content, unsigned int Type);
		//存放数据类型
		static std::string StrTranslationURL;
	};
}