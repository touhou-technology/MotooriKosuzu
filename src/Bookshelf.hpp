#pragma once
//Put Slips
#include "BambooSlips.h"

/*Put class static;
* 请仔细甄别是否有缺失
*/

//Config
//debug
std::string ConfigSlips::Path_ = "/home/awalwa/projects/Project/config/ConfigBook.json";
Json::Value ConfigSlips::ConfigJson;

//HashSlips
std::vector<dpp::snowflake*> HashSlips::ChannelSnowflake = std::vector<dpp::snowflake*>(200);
std::unique_ptr<std::unordered_map<std::string, void(*) (dpp::slashcommand_t*)>> HashSlips::SlashcommandFuntion;


//Robot
std::unique_ptr<dpp::cluster> RobotSlips::bot;

//Web
std::unique_ptr<httplib::Client> WebSlips::Translator;
std::string WebSlips::StrTranslationURL = "";
std::string WebSlips::Token = "";
std::string WebSlips::APPID = "";