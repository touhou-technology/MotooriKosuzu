﻿#pragma once
//Put Slips
#include "BambooSlips.h"

/*Put class static;
* 请仔细甄别是否有缺失
*/

//Config
//debug
std::string ConfigSlips::Path_ = "/etc/MotooriKosuzu/config/ConfigBook.json";
Json::Value ConfigSlips::ConfigJson;

//HashSlips
//同时保存
std::unique_ptr<std::unordered_map<dpp::snowflake, std::pair<dpp::snowflake, std::string>>> HashSlips::HashSnowflakeStr;
std::unique_ptr<std::unordered_map<std::string, void(*) (dpp::slashcommand_t*)>> HashSlips::SlashcommandFuntion;


//Robot
std::unique_ptr<dpp::cluster> RobotSlips::bot;
dpp::message_create_t RobotSlips::ObjMsg;

//Web
std::unique_ptr<httplib::Client> WebSlips::Translator;
std::string WebSlips::Token = "";