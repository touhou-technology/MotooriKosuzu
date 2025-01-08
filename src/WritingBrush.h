/*🖊的作用你应该晓得
* 作为书记更改
*/
#pragma once
//#include <json/json.h>
#include <dpp/dpp.h>
#include <whisper.h>

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
class PlanPen {
public:
	static void Init();

	static void OnReady();
	static void Slashcommand();
	static void AutoComplete();
	static void Message();
	static void MessageUpdate();
	static void MessageDelete();
private:
	static void SlashcommandHash(std::string command, void (*Fn)(dpp::slashcommand_t*));
	static std::vector<std::string> RegexTreatment(std::string& input);

	static void ChannlConfigBookUpdate();
};

//????我也不太清楚我要做什么（）
class LinuxPen {
public:
	static std::string cmd(const char* command);
	static void update(dpp::slashcommand_t* event);
};

class HeadPen {
public:
};

class InitVoice {
public:
	static void Init();
};

class S_TranslateVoiceConfig {
public:
	static void Init();


	static void Slashcommand();
	static void Voice();
};

class TranslateVoice {
public:
	struct Specification {
		std::string language;
		std::string model;
	};

	TranslateVoice(const Specification& Spec);

	struct promise_type {

	};

private:
	Specification m_Speci;
};