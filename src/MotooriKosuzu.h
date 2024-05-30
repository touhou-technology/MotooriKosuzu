/*是的就这样（
为bot提供接口
*/

//include discord bot
#pragma once

#include <iostream>
#include <dpp/dpp.h>


namespace MotooriKosuzu {

	//最后交给这个类来进行初始化（也就是app本体？）
	class Kosuzu {
		static void Init(std::string _Path, std::string _Token);

	private:
		//static std::unique_ptr<Kosuzu> m_instance;
	};

	//该类为读取文件内容
	class Config {
	private:
		//static std::unique_ptr<Config> m_instance;
		std::string FilePath;

	public:
		static void Init(std::string Path_);
		Config(const std::string Path);

		//保存修改内容
		bool SaveConfig();
	
	};

	//该类为DPP的库实例
	class DiscordAPI {
	private:
		static std::unique_ptr<DiscordAPI> m_instance;
		std::unique_ptr<dpp::cluster> bot;

	public:
		//初始化，且创建这个实例
		static void Init(std::string token);

		dpp::cluster* m_robot();
		DiscordAPI(std::string token);
	
	};


	
	class FnModule {
	public:

	private:

	};

}