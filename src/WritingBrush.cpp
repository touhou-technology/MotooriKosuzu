﻿#include "WritingBrush.h"
#include "BambooSlips.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <python3.11/Python.h>
#include <regex>

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
//TODO
std::string WebPen::TranslationPen(std::string text, std::string To) {
	if (text == "")
		return "";

	//static std::string cmd = "python3 API.py '" + text + "' " + To + " " + WebSlips::Token;
	//static char result[10240];
	//static char buf[1024];
	//result[10240] = { 0 };
	//buf[1024] = { 0 };
	//FILE* fp = NULL;

	//if ((fp = popen(cmd.c_str(), "r")) == NULL) {
	//	printf("popen error!\n");
	//	return "[error]";
	//}

	//while (fgets(buf, sizeof(buf), fp)) {
	//	strcat(result, buf);
	//}

	//return std::string(result);

	Py_Initialize();  // Initialize the Python interpreter

	// Add the directory containing your Python script to the Python path
	PyObject* sys_path = PySys_GetObject("path");
	PyList_Append(sys_path, PyUnicode_FromString("/home/awalwa/projects/Project")); // 替换为你的目录

	// Import the translation module
	PyObject* pName = PyUnicode_FromString("deepl_translate"); // 这里只用模块名
	PyObject* pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != nullptr) {
		// Get the translate_text function
		PyObject* pFunc = PyObject_GetAttrString(pModule, "translate_text");

		if (pFunc && PyCallable_Check(pFunc)) {
			// Prepare arguments
			PyObject* pArgs = PyTuple_Pack(3,
				PyUnicode_FromString(WebSlips::Token.c_str()),
				PyUnicode_FromString(text.c_str()),
				PyUnicode_FromString(To.c_str()));

			// Call the function
			PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);

			if (pValue != nullptr) {
				// Convert the result to a string
				std::string result = PyUnicode_AsUTF8(pValue);
				Py_DECREF(pValue);
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				Py_Finalize();  // Cleanup the Python interpreter
				return result;
			}
			else {
				PyErr_Print();
				std::cerr << "Call failed" << std::endl;
			}
		}
		else {
			PyErr_Print();
			std::cerr << "Cannot find function 'translate_text'" << std::endl;
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
		std::cerr << "Failed to load 'deepl_translate'" << std::endl;
	}

	Py_Finalize();  // Cleanup the Python interpreter
	return "";
}

void PlanPen::Init() {
	OnReady();
	Slashcommand();
	AutoComplete();
	Message();
	MessageUpdate();
	MessageDelete();
}

//读取jsoncpp的
void PlanPen::OnReady() {
	RobotSlips::bot->on_ready([](const dpp::ready_t event) {
		//RobotSlips::bot->global_bulk_command_delete();
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
			RobotSlips::bot->global_command_create(dpp::slashcommand("翻訳の開始", "コマンドで使用されるチャネルメッセージから指定されたチャネルと言語への翻訳", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_channel, "翻至", "翻訳するチャンネル（サブエリア）IDを入力", true))
				.add_option(dpp::command_option(dpp::co_string, "译至", "入力にはどの言語に翻訳する必要がありますか（どの言語を出力するか）", true).set_auto_complete(true))
			);

			RobotSlips::bot->global_command_create(dpp::slashcommand("翻訳を双方向に開く", "一次性开启2个翻译频道", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_channel, "翻至", "输入要翻译到的频道（子区）ID", true))
				.add_option(dpp::command_option(dpp::co_string, "译", "翻訳が必要な言語を入力する（その言語を入力する）", true).set_auto_complete(true))
				.add_option(dpp::command_option(dpp::co_string, "译至", "入力にはどの言語に翻訳する必要がありますか（どの言語を出力するか）", true).set_auto_complete(true))
			);

			RobotSlips::bot->global_command_create(dpp::slashcommand("翻訳の停止", "翻訳を停止する", RobotSlips::bot->me.id));
			RobotSlips::bot->global_command_create(dpp::slashcommand("双方向翻訳の停止", "双方向翻訳の停止", RobotSlips::bot->me.id));
		}
		});
}

void PlanPen::Slashcommand() {
	SlashcommandHash("翻訳の開始", [](dpp::slashcommand_t* event)->void {
		//将数据存入哈希表
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("わかった");
		else
			event->reply("はい、リダイレクトチャンネル");

		dpp::command_interaction cmd_data = event->command.get_command_interaction();

		dpp::snowflake  channel = std::get<dpp::snowflake>(event->get_parameter("翻至"));
		std::string To = std::get<std::string>(event->get_parameter("译至"));


		(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>(channel, To);
		});

	SlashcommandHash("翻訳を双方向に開く", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("わかった");
		else {
			event->reply("はい、リダイレクトチャンネル");
			//将原本的翻译指代空值
			(*HashSlips::HashSnowflakeStr)[(*HashSlips::HashSnowflakeStr)[event->command.channel_id].first] = std::pair<dpp::snowflake, std::string>();
		}

		dpp::command_interaction cmd_data = event->command.get_command_interaction();

		dpp::snowflake  channel = std::get<dpp::snowflake>(event->get_parameter("翻至"));
		std::string This_channel = std::get<std::string>(event->get_parameter("译"));
		std::string To = std::get<std::string>(event->get_parameter("译至"));

		//建立双向链接
		(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>(channel, To);
		(*HashSlips::HashSnowflakeStr)[channel] = std::pair<dpp::snowflake, std::string>(event->command.channel_id, This_channel);
		});

	//停下翻译
	SlashcommandHash("翻訳の停止", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("翻訳が開始されていないチャンネルのこと");
		else {
			event->reply("わかった");
			(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>();
		}
		});

	SlashcommandHash("双方向翻訳の停止", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>() && (*HashSlips::HashSnowflakeStr)[(*HashSlips::HashSnowflakeStr)[event->command.channel_id].first] == std::pair<dpp::snowflake, std::string>()) {
			event->reply("翻訳が開始されていないチャンネルのこと");
		}
		else {
			event->reply("わかった");
			RobotSlips::bot->message_create(dpp::message("翻訳を停止する")
				.set_channel_id((*HashSlips::HashSnowflakeStr)[event->command.channel_id].first));

			//后面需要前面的数据
			(*HashSlips::HashSnowflakeStr)[(*HashSlips::HashSnowflakeStr)[event->command.channel_id].first] = std::pair<dpp::snowflake, std::string>();
			(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>();
		}
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
	RobotSlips::bot->on_autocomplete([](const dpp::autocomplete_t& event) {
		//if (event.command.get_command_name() != "翻译至")
		//	return;

		

		dpp::command_option opt = event.options[0];

		dpp::interaction_response AutoType(dpp::ir_autocomplete_reply);

		//TODO
		for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"].size(); ++i)
			AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].asString(), ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["language"].asString()));

		RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
		});
}

//这里是处理发送消息转义的
void PlanPen::Message() {
	//同步翻译的
	RobotSlips::bot->on_message_create([](const dpp::message_create_t& event) {
		//debug
		std::cout << event.msg.to_json() << std::endl;

		//单向翻译监测是否有
		if ((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first == 0 || event.msg.author.id == RobotSlips::bot->me.id)
			return;

		//build object
		nlohmann::json data = event.msg.to_json();

		//create temp Text url
		std::string TextMsg = event.msg.content;

		std::vector<std::string> Treatment = RegexTreatment(TextMsg);

		dpp::embed ObjEmbed = dpp::embed()
			.set_color(dpp::colors::yellow)
			.set_author(event.msg.author.global_name, "", event.msg.author.get_avatar_url());

		//create to object
		dpp::message TrText = dpp::message()
			.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first);

		//调用翻译
		ObjEmbed
			.set_description(
				WebPen::TranslationPen(TextMsg, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second)
			);

		TrText.add_embed(std::move(ObjEmbed));

		//旧版string创建
		//std::string TranslateMsg = event.msg.author.global_name + ":" + WebPen::TranslationPen(event.msg.content, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second);

		//TrText.content += TranslateMsg;

		//message_reference
		if (data["message_reference"]["message_id"] != nullptr) {
			uint64_t value = std::stoull((std::string)data["message_reference"]["message_id"]);
			TrText.set_reference((*HashSlips::HashSnowflakeStr)[(dpp::snowflake)value].first);
		}

		RobotSlips::bot->message_create(TrText);

		//下面为杂项

		//附件
		for (const auto& obj : data["attachments"]) {
			TrText.content += obj["url"];
			RobotSlips::bot->message_create(dpp::message(obj["url"])
				.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first));
		}

		//url
		for (const auto& temp : Treatment) {
			RobotSlips::bot->message_create(dpp::message(temp)
				.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first));
		}

		//建立对等链接
		RobotSlips::ObjMsg = event;
		});

	//检测消息是否于翻译的消息相同
	RobotSlips::bot->on_message_create([](dpp::message_create_t BotMsg) {
		//追加进哈希表，如有一些修改即可同步
		if (BotMsg.msg.author.id == RobotSlips::bot->me.id) {
			(*HashSlips::HashSnowflakeStr)[RobotSlips::ObjMsg.msg.id] = std::pair<dpp::snowflake, std::string>(BotMsg.msg.id, (*HashSlips::HashSnowflakeStr)[BotMsg.msg.channel_id].second);

			(*HashSlips::HashSnowflakeStr)[BotMsg.msg.id] = std::pair<dpp::snowflake, std::string>(RobotSlips::ObjMsg.msg.id, (*HashSlips::HashSnowflakeStr)[RobotSlips::ObjMsg.msg.channel_id].second);
		}

		});
}

void PlanPen::MessageUpdate() {
	RobotSlips::bot->on_message_update([](const dpp::message_update_t event) {
		if ((*HashSlips::HashSnowflakeStr)[event.msg.id].first == 0 || event.msg.author.global_name == "")
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

std::vector<std::string> PlanPen::RegexTreatment(std::string& input) {
	std::vector<std::string> treatment;

	//std::regex url_regex(R"(https?://[^\s/$.?#].[^\s]*)");
	//std::smatch url_match;

	//// 迭代匹配到的 URL 链接
	//while (std::regex_search(input, url_match, url_regex)) {
	//	// 保存匹配到的 URL 链接
	//	treatment.push_back(url_match.str(0));

	//	// 从原始字符串中去除匹配到的 URL 链接
	//	input = input.substr(0, url_match.position()) + input.substr(url_match.position() + url_match.length());
	//}

	//std::regex pattern(R"(<:([^:]+):([^>]+)>)");
	//std::smatch matches;

	//while (std::regex_search(input, matches, pattern)) {
	//	// 保存匹配到的 URL 链接
	//	treatment.push_back(matches.str(0));

	//	// 从原始字符串中去除匹配到的 URL 链接
	//	input = input.substr(0, matches.position()) + input.substr(matches.position() + matches.length());
	//}


	std::vector<std::string> RegexStr = {
		R"(https?://[^\s/$.?#].[^\s]*)",
		R"(<:([^:]+):([^>]+)>)"
	};

	for (const std::string& Str : RegexStr) {
		std::regex pattern(Str);
		std::smatch matches;

		while (std::regex_search(input, matches, pattern)) {
			// 保存匹配到的 URL 链接
			treatment.push_back(matches.str(0));

			// 从原始字符串中去除匹配到的 URL 链接
			input = input.substr(0, matches.position()) + input.substr(matches.position() + matches.length());
		}
	}

	return treatment;
}

//TODO:add new Pen