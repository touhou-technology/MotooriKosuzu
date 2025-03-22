/*
* 这个文件是针对文本处理（也就是IO）的类似思想
* 因为是 "笔" 嘛.
* 其实似乎还是有点抽象了
* 
* 现在尝试将Pen作为代理类（大概）
*/
#include "WritingBrush.h"
#include "BambooSlips.h"
#include "Bookshelf.hpp"
#include "Stickers.h"

//lib
#include <httplib.h>
#include <curl/curl.h>
#include <whisper.h>

//STD
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <random>

using namespace std;

void InitPen::Init() {
	//应该最先初始化，因为其他依赖于这个
	ConfigPen::Init();
	HashPen::Init();

	WebPen::Init();
	//先要写什么再写什么
	RobotPen::Init();

	UsePen::Init();
}

void ConfigPen::Init() {
	ConfigSlips::ConfigJson = ReadFileJson(ConfigSlips::Path_);
}

//类Init御用（
std::string ConfigPen::InitPen(std::string ClassName, std::string obtain) {
	return ConfigSlips::ConfigJson[ClassName][obtain].get<std::string>();
}

nlohmann::json ConfigPen::ReadFileJson(std::string& Path) {
	ifstream File(Path);

	if (!File.is_open()) {
		cerr << "[ERROR]:Cennt open file";
	}
	else {
		nlohmann::json root;
		File >> root;
		return root;
	}

	return nlohmann::json();
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

void RobotPen::StartDebug() {
	RobotSlips::bot->on_log(dpp::utility::cout_logger());
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

[[nodiscard]]
nlohmann::json WebPen::TranslationPen(std::string text, std::string To) {
	//处理其他事件为空的情况下
	if (text == "") {
		nlohmann::json empty;
		empty["translations"][0]["text"] = "";
		empty["translations"][0]["detected_source_language"] = "empty";
		return empty;
	}

	CURL* curl;
	CURLcode res;
	std::string readBuffer;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://api-free.deepl.com/v2/translate");
		struct curl_slist* headers = NULL;

		std::string head = "Authorization: DeepL-Auth-Key";
		head = head + " " + ConfigSlips::ConfigJson["WebPen"]["Token"].get<std::string>();

		headers = curl_slist_append(headers, head.c_str());
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		std::string postData = R"({"text": [")" + text + R"("], "target_lang": ")" + To + R"("})";

		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), postData);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

		// 设置回调函数以接收响应数据
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);

		// 检查请求是否成功
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}
		else {
			// 输出响应数据
			RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), readBuffer);

		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return nlohmann::json::parse(readBuffer);
}

[[nodiscard]]
size_t WebPen::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
	size_t newLength = size * nmemb;
	try {
		s->append((char*)contents, newLength);
	}
	catch (std::bad_alloc& e) {
		// 处理内存不足的情况
		return 0;
	}
	return newLength;
}

void UsePen::Init() {
	//OnReady();
	Slashcommand();
	AutoComplete();
	Message();
	MessageUpdate();
	MessageDelete();
}

//读取jsoncpp的
void UsePen::OnReady() {
	//bot on_read
	RobotSlips::bot->on_ready([](const dpp::ready_t event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			/*コマンドを変更せずに解釈を変更してください
			コマンド変更なのでスラッシュコマンドでは使用できません*/
			RobotSlips::bot->global_command_create(dpp::slashcommand("翻訳の開始", "コマンドで使用されるチャネルメッセージから指定されたチャネルと言語への翻訳", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_channel, "翻至", "翻訳するチャンネル（サブエリア）IDを入力", true))
				.add_option(dpp::command_option(dpp::co_string, "译至", "入力にはどの言語に翻訳する必要がありますか（どの言語を出力するか）", true).set_auto_complete(true))
			);

			RobotSlips::bot->global_command_create(dpp::slashcommand("翻訳を双方向に開く", "2つの翻訳チャンネルを一度にオープン", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_channel, "翻至", "翻訳するチャンネル（サブエリア）IDを入力", true))
				.add_option(dpp::command_option(dpp::co_string, "译", "翻訳が必要な言語を入力する（その言語を入力する）", true).set_auto_complete(true))
				.add_option(dpp::command_option(dpp::co_string, "译至", "入力にはどの言語に翻訳する必要がありますか（どの言語を出力するか）", true).set_auto_complete(true))
			);

			RobotSlips::bot->global_command_create(dpp::slashcommand("翻訳の停止", "翻訳を停止する", RobotSlips::bot->me.id));
			RobotSlips::bot->global_command_create(dpp::slashcommand("双方向翻訳の停止", "双方向翻訳の停止", RobotSlips::bot->me.id));

			//);//End
		}//If End;
		});//END

	//
	nlohmann::json ObjectArray = ConfigSlips::ConfigJson["HashSlips"]["channl"];
	for (int iter = 0; iter < ObjectArray.size(); ++++ ++iter) {
		dpp::snowflake  channel = ObjectArray[iter + 1].get<dpp::snowflake>();
		std::string To = ObjectArray[iter + 2].get<std::string>();

		(*HashSlips::HashSnowflakeStr)[ObjectArray[iter].get<dpp::snowflake>()] = std::pair<dpp::snowflake, std::string>(channel, To);

		std::cout << ObjectArray[iter].get<dpp::snowflake>() << ":" << channel << ":" << To << std::endl;
	}
}

void UsePen::Slashcommand() {
	SlashcommandHash("翻訳の開始", [](dpp::slashcommand_t* event)->void {
		//将数据存入哈希表
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("わかった");
		else
			event->reply("はい、リダイレクトチャンネル");;

		dpp::snowflake channel = std::get<dpp::snowflake>(event->get_parameter("翻至"));
		std::string To = std::get<std::string>(event->get_parameter("译至"));

		(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>(channel, To);

		ChannlConfigBookUpdate();
		});

	SlashcommandHash("翻訳を双方向に開く", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("わかった");
		else {
			event->reply("はい、リダイレクトチャンネル");
			//将原本的翻译指代空值
			(*HashSlips::HashSnowflakeStr)[(*HashSlips::HashSnowflakeStr)[event->command.channel_id].first] = std::pair<dpp::snowflake, std::string>();
		}

		//dpp::command_interaction cmd_data = event->command.get_command_interaction();

		dpp::snowflake  channel = std::get<dpp::snowflake>(event->get_parameter("翻至"));
		std::string This_channel = std::get<std::string>(event->get_parameter("译"));
		std::string To = std::get<std::string>(event->get_parameter("译至"));

		//建立双向链接
		(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>(channel, To);
		(*HashSlips::HashSnowflakeStr)[channel] = std::pair<dpp::snowflake, std::string>(event->command.channel_id, This_channel);

		ChannlConfigBookUpdate();
		});

	//停下翻译
	SlashcommandHash("翻訳の停止", [](dpp::slashcommand_t* event)->void {
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("翻訳が開始されていないチャンネルのこと");
		else {
			event->reply("わかった");
			(*HashSlips::HashSnowflakeStr)[event->command.channel_id] = std::pair<dpp::snowflake, std::string>();

			ChannlConfigBookUpdate();
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

			ChannlConfigBookUpdate();
		}
		});

	RobotSlips::bot->on_slashcommand([](dpp::slashcommand_t event) {
		(*HashSlips::SlashcommandFuntion)[event.command.get_command_name()](&event);
		});
}//slashcommand end

//建立哈希索引
void UsePen::SlashcommandHash(std::string command, void(*Funtion)(dpp::slashcommand_t*)) {
	(*HashSlips::SlashcommandFuntion)[command] = Funtion;
}

void UsePen::AutoComplete() {
	RobotSlips::bot->on_autocomplete([](const dpp::autocomplete_t& event) {
		if (event.name != "翻訳を双方向に開く" && event.name != "翻訳の開始")
			return;

		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), event.name);

		dpp::interaction_response AutoType(dpp::ir_autocomplete_reply);

		for (auto& opt : event.options) {
			if (opt.focused) {
				std::string uservalue = std::get<std::string>(opt.value);

				if (uservalue == "") {
					for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"].size(); ++i) {
						AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].get<std::string>(), ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["language"].get<std::string>()));
					}
					RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
					return;
				}

				//TODO
				for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"].size(); ++i) {
					if (ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].get<std::string>().find(uservalue) != -1)
						AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].get<std::string>(), ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["language"].get<std::string>()));
				}
				RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
			}
		}
		});
}

//这里是处理发送消息转义的
void UsePen::Message() {
	//同步翻译的
	RobotSlips::bot->on_message_create([](const dpp::message_create_t& event) {
		//单向翻译监测是否有
		if ((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first == 0 || event.msg.author.id == RobotSlips::bot->me.id)
			return;

		//开始建立翻译

		//build object
		nlohmann::json data = event.msg.to_json();

		//message_referencea
		dpp::snowflake re = NULL;

		if (data["message_reference"]["message_id"] != nullptr) {
			uint64_t value = std::stoull((std::string)data["message_reference"]["message_id"]);

			dpp::message tmp;

			//TODO: 正则表达式匹配对应的源消息
			RobotSlips::bot->message_get(
				event.msg.message_reference.message_id,
				event.msg.channel_id,
				[&](const dpp::confirmation_callback_t& event) {
					std::get<dpp::message>(event.value).build_json();

				}
			);
		}

		//create temp Text url
		std::string TextMsg = event.msg.content;

		//url做处理
		std::vector<std::string> Treatment = RegexTreatment(TextMsg);

		//处理字符串
		std::stringstream ss;
		for (char ch : TextMsg)
			if (ch == '"')
				ss << "\\\"";
			else if (ch == '\n')
				ss << "\\n";
			else
				ss << ch;

		//set base
		dpp::embed ObjEmbed = dpp::embed()
			.set_color(ColorPen(event.msg.guild_id, event.msg.channel_id))
			.set_author(event.msg.author.global_name, "", event.msg.author.get_avatar_url());

		//get
		auto MessageObj = WebPen::TranslationPen(std::move(ss.str()), (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second)["translations"][0];

		if (MessageObj["detected_source_language"].get<std::string>() != "empty") {
			ObjEmbed
				.set_description(event.msg.content + "\n[☯](" + event.msg.get_url() + ")")
				.add_field("", std::move(MessageObj["text"].get<std::string>()))
				.set_footer(
					dpp::embed_footer()
					.set_text("⚝:>" + MessageObj["detected_source_language"].get<std::string>() + "->" + (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second)
				).set_timestamp(time(0));
		}

		dpp::message TrText = dpp::message()
			.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first)
			.add_embed(std::move(ObjEmbed));

		//引用
		if (re != NULL) {
			TrText.set_reference(re);
		}

		RobotSlips::bot->message_create(TrText);

		//下面为杂项

		//附件
		for (const auto& obj : data["attachments"]) {
			TrText.content += obj["url"];
			RobotSlips::bot->message_create(dpp::message(obj["url"].get<std::string>()).set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first));
		}

		//url
		for (const auto& temp : Treatment) {
			RobotSlips::bot->message_create(dpp::message(temp)
				.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first));
		}

		//std::cout << event.msg << std::endl;

		//建立对等链接
		RobotSlips::ObjMsg = event;
		});

	//TODO:将存放hash变更为查询
	//检测消息是否于翻译的消息相同
	//RobotSlips::bot->on_message_create([](dpp::message_create_t BotMsg) {
	//	//追加进哈希表，如有一些修改即可同步
	//	if (BotMsg.msg.author.id == RobotSlips::bot->me.id) {
	//		(*HashSlips::HashSnowflakeStr)[RobotSlips::ObjMsg.msg.id] = std::pair<dpp::snowflake, std::string>(BotMsg.msg.id, (*HashSlips::HashSnowflakeStr)[BotMsg.msg.channel_id].second);

	//		(*HashSlips::HashSnowflakeStr)[BotMsg.msg.id] = std::pair<dpp::snowflake, std::string>(RobotSlips::ObjMsg.msg.id, (*HashSlips::HashSnowflakeStr)[RobotSlips::ObjMsg.msg.channel_id].second);
	//	}
	//	});
}

//TODO:更新用户编辑消息
void UsePen::MessageUpdate() {
	RobotSlips::bot->on_message_update([](const dpp::message_update_t event) {
		if ((*HashSlips::HashSnowflakeStr)[event.msg.id].first == 0 || event.msg.author.global_name == "")
			return;

		dpp::message msg(event.msg.author.global_name + ":" + WebPen::TranslationPen(event.msg.content, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second)["translations"][0]["text"].get<std::string>());

		msg.set_reference((*HashSlips::HashSnowflakeStr)[event.msg.id].first)
			.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first);

		RobotSlips::bot->message_create(msg);
		});
}

void UsePen::MessageDelete() {
	RobotSlips::bot->on_message_delete([](const dpp::message_delete_t event) {
		if ((*HashSlips::HashSnowflakeStr)[event.id].first == 0)
			return;

		RobotSlips::bot->message_delete((*HashSlips::HashSnowflakeStr)[event.id].first, (*HashSlips::HashSnowflakeStr)[event.channel_id].first);

		(*HashSlips::HashSnowflakeStr)[event.id] = std::pair<dpp::snowflake, std::string>();
		});
}

std::vector<std::string> UsePen::RegexTreatment(std::string& input) {
	std::vector<std::string> treatment;

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

[[nodiscard]]
inline uint32_t UsePen::ColorPen(dpp::snowflake guild_id, dpp::snowflake channel_id) {
	std::mt19937 rng(static_cast<uint32_t>(guild_id));
	rng.discard(channel_id % 100);
	std::uniform_int_distribution<std::mt19937::result_type> disk(0, 0xFFFFFF);
	return disk(rng);
}

void UsePen::ChannlConfigBookUpdate() {
	nlohmann::json Channl;
	for (auto Obj : (*HashSlips::HashSnowflakeStr)) {
		if (Obj.first == NULL)
			continue;
		else if (Obj.second.first == NULL)
			continue;
		else if (Obj.second.second == "")
			continue;

		Channl.push_back((uint64_t)Obj.first);
		Channl.push_back((uint64_t)Obj.second.first);
		Channl.push_back(Obj.second.second);
	}

	ConfigSlips::ConfigJson["HashSlips"]["channl"] = std::move(Channl);

	std::ofstream outFile(ConfigSlips::Path_, std::ofstream::trunc); // 使用trunc模式覆盖原文件
	if (!outFile.is_open()) {
		std::cerr << "Failed to open file for writing" << std::endl;
	}

	outFile << ConfigSlips::ConfigJson.dump(4);
	outFile.close();
}

