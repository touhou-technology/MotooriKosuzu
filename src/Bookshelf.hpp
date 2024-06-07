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

//Robot
std::unique_ptr<dpp::cluster> RobotSlips::bot;

//Web
std::string WebSlips::StrTranslationURL = "";
std::string WebSlips::Token = "";
std::string WebSlips::APPID = "";