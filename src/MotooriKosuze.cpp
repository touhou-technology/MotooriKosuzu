

#include "MotooriKosuze.h"

//类名就是包括该类的作用
namespace MotooriKosuze {

	void Kosuze::Init() {
		DiscordAPI::Init();
		Config::Init();
	}



	void Config::Init(std::string Path_){
		m_instance.reset(new Config(Path_));
	}

	Config::Config(const std::string Path) 
		:FilePath(Path){

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