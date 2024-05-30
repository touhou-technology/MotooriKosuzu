/*是的就这样（
为bot提供接口
*/

//include discord bot
#pragma once
#include <dpp/dpp.h>
#include <jsoncpp/json/json.h>

namespace MotooriKosuze {

	//最后交给这个类来进行初始化（也就是app本体？）
	class Kosuze {
		static void Init();

	private:
		static std::unique_ptr<Kosuze> m_instance;
	};

	//该类为读取文件内容
	class Config {
	public:
		static void Init();
		Config();

		

		//保存修改内容
		bool SaveConfig();

		friend class DiscordAPI;

	private:
		static std::unique_ptr<Config> m_instance;
		std::string FilePath;
	};

	//该类为DPP的库实例
	class DiscordAPI {
	public:
		//初始化，且创建这个实例
		static void Init(std::string token);


		dpp::cluster* m_robot();
		DiscordAPI(std::string token);
	private:
		static std::unique_ptr<DiscordAPI> m_instance;
		dpp::cluster bot;
	};


	
	class FnModule {
	public:

	private:

	};

}