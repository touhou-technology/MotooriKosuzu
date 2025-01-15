#pragma once
#include <dpp/dpp.h>

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

	};
public:
	//kay -> user_params
	void AddUser(dpp::snowflake obj, user_params params);
	void AddUser(dpp::snowflake obj, user_params&& params);
	void DelUser(dpp::snowflake obj);

	void SetFlag();
public:
	TranslateVoice();

private:
	std::vector<user_params> m_object;
	//thread
	bool flag;
};