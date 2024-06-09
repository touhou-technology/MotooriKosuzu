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
	static std::string InitPen(std::string ClassName, std::string obtain);
	static Json::Value ReadFileJson(std::string Path);
	static Json::Value GetConfigJson();
};

class HashPen {
public:
	static void Init();
};

class RobotPen {
public:
	static void Init();
	static void Start();
	static void work(void (*Fn)(dpp::cluster* bot));
	static dpp::cluster* GetBot();
};

class WebPen {
public:
	//Pen初始化对应的竹木简牍,从config读取
	static void Init();

	static void SetTranslator(std::string URL);

	//web 翻译之类的
	static std::string TranslationPen(std::string text, std::string To);

	//Webhook
	static void Webhook();
};

//运行需要安排的任务
class PlanPen {
public:
	static void Init();

	static void OnReady();
	static void Slashcommand();
	static void SlashcommandHash(std::string command, void (*Fn)(dpp::slashcommand_t*));
	static void AutoComplete();
	static void Message();
	static void MessageUpdate();
	static void MessageDelete();
	static void WebhookCreate();
};