#ifndef BAMBOOSLIPS_H
#define BAMBOOSLIPS_H

#include <dpp/dpp.h>
#include <httplib.h>
#include <memory>
#include <unordered_map>
// #include "Dictation.h"

class ConfigSlips {
  public:
    inline static std::string Path_ = "/etc/MotooriKosuzu/config/ConfigBook.json";
	inline static nlohmann::json ConfigJson = {};
};

class HashSlips {
  public:
	inline static std::unique_ptr<std::unordered_map<
		dpp::snowflake, std::pair<dpp::snowflake, std::string>>>
		HashSnowflakeStr = {};
	inline static std::unique_ptr<
		std::unordered_map<std::string, void (*)(dpp::slashcommand_t *)>>
		SlashcommandFuntion = {};
};

class RobotSlips {
  public:
	inline static std::unique_ptr<dpp::cluster> bot = {};
	inline static dpp::message_create_t ObjMsg = {};
};

class WebSlips {
  public:
	enum { TranslationURL = 0, Link };

	inline static std::unique_ptr<httplib::Client> Translator = {};
	inline static std::string Token = {};
};

class VoiceSlips {
  public:
	// inline static std::unique_ptr<TranslateVoice> S_TranslateVoice;
};

#endif /* BAMBOOSLIPS_H */
