

#include "MotooriKosuze.h"
#include <functional>
//类名就是包括该类的作用
namespace MotooriKosuze {
	//该函数为为类初始化
	void Kosuze::Init() {
		m_instance.reset(new Kosuze);
		DiscordAPI::Init();
		Config::Init();
		
	}



	void Config::Init(){
		m_instance.reset(new Config());
	}

	Config::Config() {

	}

	//保存配置
	bool Config::SaveConfig() {


	}

	//DiscordAPI段的实现
	void DiscordAPI::Init(std::string token = /*这是测试用接口更换不同的令牌*/) {
		m_instance.reset(new DiscordAPI(token));
	}

	//返回一个robot以获取额，似乎可以用友元
	dpp::cluster* DiscordAPI::m_robot() {
		return &bot;
	}

	DiscordAPI::DiscordAPI(std::string token)
		:bot(token, dpp::i_default_intents | dpp::i_message_content) {

		//启动日志
		bot.on_log(dpp::utility::cout_logger());

		//启动以线程阻塞方式
		bot.start(dpp::st_wait);
	}

}