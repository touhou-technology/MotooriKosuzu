/*🖊的作用你应该晓得
* 作为书记更改
*/
#pragma once
#include <json/json.h>
#include <httplib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

//配置
namespace Pen {
	class ConfigPen {
	public:
		//
		static void init();
		static Json::Value ReadFile(std::string Path);

	private:
		std::unique_ptr<ConfigPen> m_ConfigPen;
	};

	class WebPen {
	public:
		//web 翻译之类的
		static std::string TranslationPen(std::string TStr);

	private:

	};
}