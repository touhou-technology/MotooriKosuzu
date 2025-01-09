/*🖊的作用你应该晓得
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
	static void Message();
	static void MessageUpdate();
	static void MessageDelete();
private:
	static void SlashcommandHash(std::string command, void (*Fn)(dpp::slashcommand_t*));
	static std::vector<std::string> RegexTreatment(std::string& input);

	static uint32_t ColorPen(dpp::snowflake guild_id, dpp::snowflake channel_id);
	static void ChannlConfigBookUpdate();
};

//????我也不太清楚我要做什么（）
class LinuxPen {
public:
	static std::string cmd(const char* command);
	static void update(dpp::slashcommand_t* event);
};

class InitVoice {
public:
	static void Init();
};

//static(Not Pen)之后考虑移出
class TranslateVoice {
public:
	struct Specification {
		std::string language;
		std::string model;
		dpp::snowflake ID;
	};

	struct promise_type {
		std::suspend_never initial_suspend() {
			return{};
		}

		std::suspend_never final_suspend() noexcept {
			return{};
		}

		std::suspend_always await() {

		}

		TranslateVoice get_return_object() {
			return TranslateVoice{ std::coroutine_handle<promise_type>::from_promise(*this) };
		}

		void return_void() {};
	};

	TranslateVoice(std::coroutine_handle<promise_type> handle_) :handle(handle_) {};

	void Send(const dpp::voice_receive_t& event);

private:
	Specification m_Speci;
	std::coroutine_handle<promise_type> handle;
};

class S_TranslateVoiceConfig {
public:
	static void Init();

private:
	static void Slashcommand();
	static void Voice();
};