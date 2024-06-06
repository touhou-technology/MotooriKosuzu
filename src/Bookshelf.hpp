#pragma once
//Put Slips
#include "BambooSlips.h"

/*Put class static;
* 
*/

//Config
std::string ConfigSlips::Path_ = "../config/ConfigBook.json";
Json::Value ConfigSlips::ConfigJson = NULL;

//Web
std::string WebSlips::StrTranslationURL = "";