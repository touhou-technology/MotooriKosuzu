#pragma once
#include <dpp/dpp.h>

#include <jsoncpp/json/json.h>

/*
自定义
*/

//最后交给这个类来进行初始化（也就是app本体？）
namespace MoKu {
	class Kosuze {
	public:
		static void Init();
		Kosuze();
		~Kosuze();
	private:
		static std::unique_ptr<Kosuze> m_Kosuze;
	};

	//该类为读取文件内容
	class ConfigFile {
	public:
		static void Init();
		static Json::Value* GetJSObj();

		//readFile
		Json::Value ReadFile();

		ConfigFile();
		~ConfigFile();

	private:
		static std::unique_ptr<ConfigFile> m_ConfigFile;
		std::string FilePath;
		Json::Value JSObj;
	};

	//该类为DPP的库实例
	class DiscordAPI {
	public:
		//初始化，且创建这个实例
		static void Init();

		DiscordAPI(std::string token);
		~DiscordAPI();

	private:
		dpp::cluster* m_robot();
		static std::unique_ptr<DiscordAPI> m_DiscordAPI;
		dpp::cluster bot;
	};

}

