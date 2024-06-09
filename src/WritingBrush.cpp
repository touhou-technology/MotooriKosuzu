#include "WritingBrush.h"
#include "BambooSlips.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

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
	HashSlips::HashSnowflakeStr.reset(new std::unordered_map<dpp::snowflake, std::pair<dpp::snowflake, std::string>>());
	HashSlips::SlashcommandFuntion.reset(new std::unordered_map<std::string, void(*)(dpp::slashcommand_t*)>());
}

void RobotPen::Init() {
	RobotSlips::bot.reset(new dpp::cluster(ConfigPen::InitPen("RobotSlips", "Token"), dpp::i_default_intents | dpp::i_message_content | dpp::i_guild_messages));
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
	WebSlips::APPID = ConfigPen::InitPen("WebPen", " APPID");
	//use default
	//WebPen::SetTranslator();
}

void WebPen::SetTranslator(std::string URL = WebSlips::StrTranslationURL) {
	//WebSlips::Translator.reset(new httplib::Client(""));
}

//test需要翻译的文本，To是翻译成什么的
std::string WebPen::TranslationPen(std::string text, std::string To) {
	std::string cmd = "python3.10 API.py " + text + " " + To;

	char result[10240] = { 0 };
	char buf[1024] = { 0 };
	FILE* fp = NULL;

	if ((fp = popen(cmd.c_str(), "r")) == NULL) {
		printf("popen error!\n");
		return "[error]";
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(100));

	while (fgets(buf, sizeof(buf), fp)) {
		strcat(result, buf);
	}



	return std::string(result);
}

void WebPen::Webhook() {
	RobotSlips::bot->execute_webhook_sync(dpp::webhook("https://discord.com/api/webhooks/1249226023355023452/0D-dnrqO7xayTDn6tVJ7TdacGc5s_hLClVA4IjRU7beYVm4w8hsWLum8WlDmgqWPakby"),
		dpp::message("Test"));
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
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("Okey");
		else
			event->reply("Okey, Redirect Channel");

		dpp::command_interaction cmd_data = event->command.get_command_interaction();

		dpp::snowflake  channel = std::get<dpp::snowflake>(event->get_parameter("翻译的频道"));
		std::string To = std::get<std::string>(event->get_parameter("翻译至"));;


		(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>(channel, To);
		});

	//停下翻译
	SlashcommandHash("停止翻译", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("Refers to channels that have not yet started translation");
		else {
			event->reply("okey");
			(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>();
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
		if ((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first == 0 || event.msg.author.id == RobotSlips::bot->me.id)
			return;

		dpp::message TrText(WebPen::TranslationPen(event.msg.content, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second));
		TrText.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first);

		//追加进哈希表，如有一些修改即可同步
		(*HashSlips::HashSnowflakeStr)[event.msg.id] = std::pair<dpp::snowflake, std::string>(TrText.id, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second);

		//测试用，但似乎已经可以用了
		RobotSlips::bot->message_create(TrText);
		});
}

void PlanPen::MessageUpdate() {
	RobotSlips::bot->on_message_update([](const dpp::message_update_t event) {
		if ((*HashSlips::HashSnowflakeStr)[event.msg.id].first == 0 || event.msg.author.id == RobotSlips::bot->me.id)
			return;

		std::cout << (*HashSlips::HashSnowflakeStr)[event.msg.id].first;


		});
}

void PlanPen::MessageDelete() {
	RobotSlips::bot->on_message_delete([](const dpp::message_delete_t event) {
		if ((*HashSlips::HashSnowflakeStr)[event.id].first == 0 || (*HashSlips::HashSnowflakeStr)[event.channel_id].first == 0 || event.id == RobotSlips::bot->me.id)
			return;

		RobotSlips::bot->message_delete((*HashSlips::HashSnowflakeStr)[event.id].first, (*HashSlips::HashSnowflakeStr)[event.channel_id].first);

		(*HashSlips::HashSnowflakeStr)[event.id] = std::pair<dpp::snowflake, std::string>();

		});
}



void PlanPen::WebhookCreate() {

}
