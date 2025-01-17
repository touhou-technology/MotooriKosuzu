#pragma once
#include <dpp/dpp.h>
#include <unordered_map>
#include <chrono>

class InitVoice {
public:
	static void Init();
};

class S_TranslateVoiceConfig {
public:
	static void Init();

private:
	static void Slashcommand();
	static void Voice();
	static void AutoComplete();
};

class TranslateVoice {
public:
	struct user_params {
		std::string language;
		std::string model;
		dpp::snowflake ID;
		std::chrono::milliseconds flag;
		std::chrono::milliseconds time;
		FILE* vc_record;
	};
public:
	//kay -> user_params
	void AddUser(user_params params);
	void DelUser(dpp::snowflake obj);

	void SendVC(const dpp::voice_receive_t& event);
	void ResetFlag();
	//
	void Suitable();
	//
	void Understand(user_params& user);

public:
	TranslateVoice();

	//debug
	void test_add_instance();
public:
	

private:
	std::unordered_map <dpp::snowflake, user_params> m_object;
	//thread
	std::chrono::milliseconds time = std::chrono::milliseconds(500);
	std::thread a;
};

