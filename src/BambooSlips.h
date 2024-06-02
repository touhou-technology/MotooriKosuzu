#pragma once
#include <iostream>

namespace BambooSlips {
	class ConfigSlips {
	public:
		static void Init();
	private:
		static std::string str;
	};
	class WebSlips {
	public:
		static std::string GetTranslationURL();

		//存放数据类型
		static std::string TranslationURL;
	};
}