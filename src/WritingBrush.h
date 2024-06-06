/*🖊的作用你应该晓得
* 作为书记更改
*/
#pragma once
#include <json/json.h>
#include <httplib.h>
#include <dpp/dpp.h>
#include <openssl/md5.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//配置
class ConfigPen {
	public:
		//Pen初始化对应的竹木简牍,从config读取
		static void Init();
		static Json::Value ReadFileJson(std::string Path);
		static Json::Value GetConfigJson();
};

	class RobotPen {
	public:
		static void Init();
		static void work(void (*Fn)(dpp::cluster* bot));
		static dpp::cluster* GetBot();
	};

	class WebPen {
	public:
		//Pen初始化对应的竹木简牍,从config读取
		static void Init();
		//web 翻译之类的
	};
		static std::string TranslationPen(std::string TStr);
};