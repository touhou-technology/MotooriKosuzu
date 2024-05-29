#include "MotooriKosuze.h"
//类名就是包括该类的作用
namespace MotooriKosuze {


//DiscordAPI段的实现
//-----------------------
void DiscordAPI::Init(std::string) {

}

//返回一个robot以获取额，似乎可以用友元
dpp::activity* DiscordAPI::m_robot(){
	return &robot;
}
//-----------------------


//
Config::Config(const std::string Path){

}

//保存配置
bool Config::SaveConfig(){


}

}