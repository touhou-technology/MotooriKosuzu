﻿/*🖊的作用你应该晓得
* 作为书记更改
*/
#pragma once
#include <dpp/dpp.h>
#include <coroutine>

//配置

class InitPen {
public:
	static void Init();
};

class ConfigPen {
public:
	//Pen初始化对应的竹木简牍,从config读取
	static void Init();
	static std::string InitPen(std::string ClassName, std::string obtain);
	static nlohmann::json ReadFileJson(std::string& Path);
	//static Json::Value GetConfigJson();
};

class HashPen {
public:
	static void Init();
	static void HaseCacheDelete();
};

class RobotPen {
public:
	static void Init();
	static void Start();
	static void StartDebug();
	static void work(void (*Fn)(dpp::cluster* bot));
	static dpp::cluster* GetBot();
};

class WebPen {
public:
	//Pen初始化对应的竹木简牍,从config读取
	static void Init();

	//web 翻译之类的
	static nlohmann::json TranslationPen(std::string text, std::string To);
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};

//运行需要安排的任务
class UsePen {
public:
	static void Init();

	static void OnReady();
	static void Slashcommand();
	static void AutoComplete();
	static void MessageCreate();
	static void MessageUpdate();
	static void MessageDelete();
public:
	static void SlashcommandHash(std::string command, void (*Fn)(dpp::slashcommand_t*));
private:

	inline static uint32_t ColorPen(dpp::snowflake guild_id, dpp::snowflake channel_id);
};

class StringPen {
public:
	static std::vector<std::string> RegexTreatment(std::string& input);
	static std::string CompatibleURL(std::string& Obj);
};