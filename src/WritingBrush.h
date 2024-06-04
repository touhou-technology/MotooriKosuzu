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
		//Pen初始化对应的竹木简牍,从config读取
		static void init();
		static Json::Value ReadFileJson(std::string Path);
		static Json::Value GetConfigJson();
	};

	class WebPen {
	public:
		//Pen初始化对应的竹木简牍,从config读取
		static void init();
		//web 翻译之类的
		static std::string TranslationPen(std::string TStr);
	};
}