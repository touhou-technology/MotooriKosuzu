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
	WebSlips::Token = ConfigPen::InitPen("WebPen", "Token");
}

//test需要翻译的文本，To是翻译成什么的
std::string WebPen::TranslationPen(std::string text, std::string To) {
	std::string cmd = "python3.10 API.py " + text + " " + To + " " + WebSlips::Token;

	char result[10240] = { 0 };
	char buf[1024] = { 0 };
	FILE* fp = NULL;

	if ((fp = popen(cmd.c_str(), "r")) == NULL) {
		printf("popen error!\n");
		return "[error]";
	}

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
	AutoComplete();
	Message();
	MessageUpdate();
	MessageDelete();
	WebhookCreate();
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
				.add_option(dpp::command_option(dpp::co_channel, "翻译到频道", "输入要翻译到的频道（子区）ID", true))
				.add_option(dpp::command_option(dpp::co_string, "翻译至", "输入需要翻译到什么语言", true).set_auto_complete(true))
			);

			RobotSlips::bot->global_command_create(dpp::slashcommand("开启翻译双向", "一次性开启2个翻译频道", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_channel, "翻译到频道", "输入要翻译到的频道（子区）ID", true))
				.add_option(dpp::command_option(dpp::co_string, "翻译", "输入需要翻译的什么语言", true).set_auto_complete(true))
				.add_option(dpp::command_option(dpp::co_string, "翻译至", "输入需要翻译到什么语言", true).set_auto_complete(true))
			);

			RobotSlips::bot->global_command_create(dpp::slashcommand("停止翻译", "停下", RobotSlips::bot->me.id));
			RobotSlips::bot->global_command_create(dpp::slashcommand("停止翻译双向", "停下", RobotSlips::bot->me.id));
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

		dpp::snowflake  channel = std::get<dpp::snowflake>(event->get_parameter("翻译到频道"));
		std::string To = std::get<std::string>(event->get_parameter("翻译至"));


		(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>(channel, To);
		});

	SlashcommandHash("开启翻译双向", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("Okey");
		else {
			event->reply("Okey, Redirect Channel");
			//将原本的翻译指代空值
			(*HashSlips::HashSnowflakeStr)[(*HashSlips::HashSnowflakeStr)[event->command.channel_id].first] = std::pair<dpp::snowflake, std::string>();
		}

		dpp::command_interaction cmd_data = event->command.get_command_interaction();

		dpp::snowflake  channel = std::get<dpp::snowflake>(event->get_parameter("翻译到频道"));
		std::string This_channel = std::get<std::string>(event->get_parameter("翻译"));
		std::string To = std::get<std::string>(event->get_parameter("翻译至"));

		//建立双向链接
		(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>(channel, To);
		(*HashSlips::HashSnowflakeStr)[channel] = std::pair<dpp::snowflake, std::string>(event->command.channel_id, This_channel);
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

	SlashcommandHash("停止翻译双向", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>() && (*HashSlips::HashSnowflakeStr)[(*HashSlips::HashSnowflakeStr)[event->command.channel_id].first] == std::pair<dpp::snowflake, std::string>()) {
			event->reply("Refers to channels that have not yet started translation");
		}
		else {
			event->reply("okey");
			RobotSlips::bot->message_create(dpp::message("Stop translation")
				.set_channel_id((*HashSlips::HashSnowflakeStr)[event->command.channel_id].first));

			//后面需要前面的数据
			(*HashSlips::HashSnowflakeStr)[(*HashSlips::HashSnowflakeStr)[event->command.channel_id].first] = std::pair<dpp::snowflake, std::string>();
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

void PlanPen::AutoComplete() {
	RobotSlips::bot->on_autocomplete([](const dpp::autocomplete_t event) {
		//if (event.command.get_command_name() != "翻译至")
		//	return;

		dpp::command_option opt = event.options[0];

		dpp::interaction_response AutoType(dpp::ir_autocomplete_reply);

		for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"].size(); ++i)
			AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].asString(), ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["language"].asString()));

		RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
		});
}

void PlanPen::Message() {
	//同步翻译的
	RobotSlips::bot->on_message_create([](dpp::message_create_t event) {
		//单向翻译（
		if ((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first == 0 || event.msg.author.id == RobotSlips::bot->me.id)
			return;

		std::string TranslateMsg = event.msg.author.global_name + ":" + WebPen::TranslationPen(event.msg.content, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second);

		dpp::message TrText(TranslateMsg);
		TrText.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first);

		RobotSlips::ObjMsg = event;

		//测试用，但似乎已经可以用了
		RobotSlips::bot->message_create(TrText);
		});

	//检测消息是否于翻译的消息相同
	RobotSlips::bot->on_message_create([](dpp::message_create_t BotMsg) {

		//追加进哈希表，如有一些修改即可同步
		if (BotMsg.msg.author.id == RobotSlips::bot->me.id)
			(*HashSlips::HashSnowflakeStr)[RobotSlips::ObjMsg.msg.id] = std::pair<dpp::snowflake, std::string>(BotMsg.msg.id, (*HashSlips::HashSnowflakeStr)[BotMsg.msg.channel_id].second);

		});
}

void PlanPen::MessageUpdate() {
	RobotSlips::bot->on_message_update([](const dpp::message_update_t event) {
		if ((*HashSlips::HashSnowflakeStr)[event.msg.id].first == 0)
			return;

		dpp::message msg(event.msg.author.global_name + ":" + WebPen::TranslationPen(event.msg.content, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second));

		msg.set_reference((*HashSlips::HashSnowflakeStr)[event.msg.id].first)
			.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first);

		RobotSlips::bot->message_create(msg);
		});
}

void PlanPen::MessageDelete() {
	RobotSlips::bot->on_message_delete([](const dpp::message_delete_t event) {
		if ((*HashSlips::HashSnowflakeStr)[event.id].first == 0)
			return;

		RobotSlips::bot->message_delete((*HashSlips::HashSnowflakeStr)[event.id].first, (*HashSlips::HashSnowflakeStr)[event.channel_id].first);

		(*HashSlips::HashSnowflakeStr)[event.id] = std::pair<dpp::snowflake, std::string>();

		});
}



void PlanPen::WebhookCreate() {

}
