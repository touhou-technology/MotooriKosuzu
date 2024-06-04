/*Put class static;
* 
*/

//Put Slips
#include "BambooSlips.h"

using namespace BambooSlips;

//Config
std::string ConfigSlips::Path_;
Json::Value ConfigSlips::ConfigJson = "./config/ConfigBook.json";

//Web
std::string WebSlips::StrTranslationURL;