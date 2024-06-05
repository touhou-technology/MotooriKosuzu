/*Put class static;
* 
*/

//Put Slips
#include "BambooSlips.h"

using namespace BambooSlips;

//Config
std::string ConfigSlips::Path_ = "/home/awalwa/projects/Project/config/ConfigBook.json";
Json::Value ConfigSlips::ConfigJson;

//Robot
dpp::cluster RobotSlips::bot;

//Web
std::string WebSlips::StrTranslationURL;