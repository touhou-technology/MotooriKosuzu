/*是的就这样（
为bot提供接口
*/

//include discord bot
#pragma once
#include <dpp/dpp.h>


namespace MotooriKosuze {

//该类为DPP的库实例
class DiscordAPI {
public:
	//
	static void Init(std::string token);
	
	dpp::activity* m_robot();
private:
	static std::unique_ptr<DiscordAPI> m_instance;
	dpp::activity robot;
};

//该类为读取文件内容
class Config {
public:
	
	Config(const std::string Path);

	//保存修改内容
	std::string Find();
	bool SaveConfig();
private:
	std::string FilePath;
};

class Web {
public:

private:

};
}