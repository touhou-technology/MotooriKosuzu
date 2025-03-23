/*
* 实验性功能
* 不稳定
* 
*/
#pragma once
#include <dpp/dpp.h>
#include <unordered_map>
#include <chrono>
#include <curl/curl.h>

class InitVoice {
public:
	static void Init();
};

class S_TranslateVoiceConfig {
public:
	static void Init();

private:
	static void OnReady();
	static void Slashcommand();
	static void Voice();
	static void AutoComplete();
};

class TranslateVoice {
public:
	struct user_params {
		std::string language;
		std::string model;
		std::string name;
		dpp::snowflake id;
		std::chrono::milliseconds time = std::chrono::milliseconds(1000);
		FILE* vc_record;
	private:
		friend TranslateVoice;
		std::chrono::milliseconds flag = std::chrono::milliseconds(0);
	};
public:
	//kay -> user_params
	void AddUser(user_params params);
	void DelUser(dpp::snowflake obj);
	void SetCout(dpp::snowflake obj);

	void SendVC(const dpp::voice_receive_t& event);
	void ResetFlag();
	//
	void Suitable();
	//
	void Understand(user_params& user);

	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

	static void loadModel(const std::string& modelPath);
	static nlohmann::json performInference(const std::string& filePath, const std::string& language);
public:
	TranslateVoice();
private:
	std::unordered_map<dpp::snowflake, user_params> m_object;
	//判断是否有人说话了
	bool flag = false;
	//thread
	std::chrono::milliseconds time = std::chrono::milliseconds(500);
	dpp::snowflake ObjCout;
};