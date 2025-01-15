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
		int time;
		FILE* record;
	};
public:
	//kay -> user_params
	void AddUser(dpp::snowflake obj, user_params params);
	void AddUser(dpp::snowflake obj, user_params&& params);
	void DelUser(dpp::snowflake obj);

	void SendVC(const dpp::voice_receive_t& event);
	void SetFlag();
	//
	void Understand();
public:
	TranslateVoice();

public:
	

private:
	std::unordered_map <dpp::snowflake, user_params> m_object;
	//thread
	bool flag;
};

