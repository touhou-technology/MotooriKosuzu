/*Put class static;
* 
*/

//Put Slips
#include "BambooSlips.h"

using namespace BambooSlips;

//Config
std::string ConfigSlips::Path_ = "../config/ConfigBook.json";
Json::Value ConfigSlips::ConfigJson;

//Web
std::string WebSlips::StrTranslationURL;