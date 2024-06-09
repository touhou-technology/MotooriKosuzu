#include "WritingBrush.h"
#include "BambooSlips.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void ConfigPen::Init() {
	ConfigSlips::ConfigJson = ReadFileJson(ConfigSlips::Path_);
}

//类Init御用（
std::string ConfigPen::InitPen(std::string ClassName, std::string obtain) {
	return ConfigPen::GetConfigJson()[ClassName][obtain].asString();
}

Json::Value ConfigPen::ReadFileJson(string Path) {
	ifstream File(Path);

	if (!File.is_open()) {
		cerr << "cennt open file";
	}

	Json::CharReaderBuilder ReaderBuilder;
	ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会编程\uxxx

	Json::Value root;

	//把文件转变为json对象
	std::string strerr;

	if (!Json::parseFromStream(ReaderBuilder, File, &root, &strerr)) {
		std::cerr << "json解析错误" << std::endl;
	}

	return root;
}

Json::Value ConfigPen::GetConfigJson() {
	return ConfigSlips::ConfigJson;
}

void HashPen::Init() {
	HashSlips::ChannelSnowflake.reset(new std::unordered_map<dpp::snowflake, std::pair<dpp::snowflake, std::string>>());
	HashSlips::SlashcommandFuntion.reset(new std::unordered_map<std::string, void(*)(dpp::slashcommand_t*)>());
}

void RobotPen::Init() {
	RobotSlips::bot.reset(new dpp::cluster(ConfigPen::InitPen("RobotSlips", "Token"), dpp::i_default_intents | dpp::i_message_content));
}

//start bot(thread wait)
void RobotPen::Start() {
	GetBot()->start(dpp::st_wait);
}

void RobotPen::work(void(*Fn)(dpp::cluster* bot)) {
	Fn(&*RobotSlips::bot);
}
//Adapt to Lambda
dpp::cluster* RobotPen::GetBot() {
	return &*RobotSlips::bot;
}

void WebPen::Init() {
	WebSlips::StrTranslationURL = ConfigPen::InitPen("WebPen", "StrTranslationURL");
	WebSlips::Token = ConfigPen::InitPen("WebPen", "Token");
	WebSlips::APPID = ConfigPen::InitPen("WebPen", "APPID");
	//use default
	//WebPen::SetTranslator();
}

void WebPen::SetTranslator(std::string URL = WebSlips::StrTranslationURL) {
	//WebSlips::Translator.reset(new httplib::Client(""));
}

//test需要翻译的文本，To是翻译成什么的
std::string WebPen::TranslationPen(std::string text, std::string To) {

	return text;
}

void WebPen::Webhook() {

}

void PlanPen::Init() {
	OnReady();
	Slashcommand();
	Message();
}

//读取jsoncpp的
void PlanPen::OnReady() {
	RobotSlips::bot->on_ready([](const dpp::ready_t event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			Json::Value ObjectArray;
			ObjectArray = ConfigPen::GetConfigJson()["slashcommand"];
			std::cout << ObjectArray.size();
			int iter_2 = 1;
			for (int iter = 0; iter != ObjectArray.size(); ++++iter) {
				std::cout << ObjectArray[iter].asString() << ":" << ObjectArray[iter_2].asString() << std::endl;
				RobotSlips::bot->global_command_create(dpp::slashcommand(ObjectArray[iter].asString(), ObjectArray[iter_2].asString(), RobotSlips::bot->me.id));
				++++iter_2;
			}
			//先这样，后续升级json的读取（）
			RobotSlips::bot->global_command_create(dpp::slashcommand("开启翻译", "启动！", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_channel, "翻译的频道", "输入要翻译的频道（子区）ID", true))
				.add_option(dpp::command_option(dpp::co_string, "翻译至", "输入需要翻译到什么语言", true))
			);

			RobotSlips::bot->global_command_create(dpp::slashcommand("停止翻译", "停下", RobotSlips::bot->me.id)
			);
		}
		});
}

void PlanPen::Slashcommand() {
	SlashcommandHash("开启翻译", [](dpp::slashcommand_t* event)->void {
		//将数据存入哈希表
		if ((*HashSlips::ChannelSnowflake)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("Okey");
		else
			event->reply("Okey, Redirect Channel");

		dpp::command_interaction cmd_data = event->command.get_command_interaction();

		dpp::snowflake  channel = std::get<dpp::snowflake>(event->get_parameter("翻译的频道"));
		std::string To = std::get<std::string>(event->get_parameter("翻译至"));;


		(*HashSlips::ChannelSnowflake)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>(channel, To);
		});

	//停下翻译
	SlashcommandHash("停止翻译", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::ChannelSnowflake)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("Refers to channels that have not yet started translation");
		else {
			event->reply("okey");
			(*HashSlips::ChannelSnowflake)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>();
		}
		});

	SlashcommandHash("ping", [](dpp::slashcommand_t* event)->void {
		event->reply("OwO");
		});

	RobotSlips::bot->on_slashcommand([](dpp::slashcommand_t event) {
		//std::cout << (*HashSlips::SlashcommandFuntion)[event.command.get_command_name()];
		//if ((*HashSlips::SlashcommandFuntion)[event.command.get_command_name()] = 0)
		//	return;
		(*HashSlips::SlashcommandFuntion)[event.command.get_command_name()](&event);
		});
}

//建立哈希索引
void PlanPen::SlashcommandHash(std::string command, void(*Funtion)(dpp::slashcommand_t*)) {
	(*HashSlips::SlashcommandFuntion)[command] = Funtion;
}

void PlanPen::Message() {
	//同步翻译的
	RobotSlips::bot->on_message_create([](dpp::message_create_t event) {
		//单向翻译（
		if ((*HashSlips::ChannelSnowflake)[event.msg.channel_id].first == 0 || event.msg.author.id == RobotSlips::bot->me.id)
			return;

		//测试用，但似乎已经可以用了
		RobotSlips::bot->message_create(dpp::message(
			WebPen::TranslationPen(event.msg.content, (*HashSlips::ChannelSnowflake)[event.msg.channel_id].second))
			.set_channel_id((*HashSlips::ChannelSnowflake)[event.msg.channel_id].first)
		);
		});
}
