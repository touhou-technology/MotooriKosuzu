/*
* 这个文件是针对文本处理（也就是IO）的类似思想
* 因为是 "笔" 嘛.
*/
#include "WritingBrush.h"
#include "BambooSlips.h"
#include "Voice.h"
#include "Bookshelf.hpp"


//
#include <httplib.h>
#include <curl/curl.h>

//C++ 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

using namespace std;

void InitPen::Init() {
	//应该最先初始化，因为其他依赖于这个
	ConfigPen::Init();
	HashPen::Init();

	WebPen::Init();
	//先要写什么再写什么
	RobotPen::Init();
	PlanPen::Init();
}

void ConfigPen::Init() {
	ConfigSlips::ConfigJson = ReadFileJson(ConfigSlips::Path_);
}

//类Init御用（
std::string ConfigPen::InitPen(std::string ClassName, std::string obtain) {
	return ConfigSlips::ConfigJson[ClassName][obtain].asString();
}

Json::Value ConfigPen::ReadFileJson(string Path) {
	ifstream File(Path);

	if (!File.is_open()) {
		cerr << "[ERROR]:Cennt open file";
	}

	Json::CharReaderBuilder ReaderBuilder;
	ReaderBuilder["emitUTF8"] = true;//utf8支持，不加这句，utf8的中文字符会变成\uxxx

	Json::Value root;

	//把文件转变为json对象
	std::string strerr;

	if (!Json::parseFromStream(ReaderBuilder, File, &root, &strerr)) {
		std::cerr << "[ERROR]:json can't read" << std::endl;
	}

	return root;
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

Json::Value WebPen::TranslationPen(std::string text, std::string To) {
	//处理其他事件为空的情况下
	if (text == "") {
		Json::Value empty;
		empty["translations"][0]["text"] = "";
		empty["translations"][0]["detected_source_language"] = "Unknow";
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
		head = head + " " + ConfigSlips::ConfigJson["WebPen"]["Token"].asCString();

		headers = curl_slist_append(headers, head.c_str());
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		std::string postData = R"({"text": [")" + text + R"("], "target_lang": ")" + To + R"("})";

		std::cout << postData << std::endl;

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
			std::cout << "Response: " << readBuffer << std::endl;
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	Json::Value root;
	Json::Reader reader;

	reader.parse(readBuffer, root);

	return root;
}

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
	//bot on_read
	RobotSlips::bot->on_ready([](const dpp::ready_t event) {
		//RobotSlips::bot->global_bulk_command_delete();
		//(),需要添加新的解析
		if (dpp::run_once<struct register_bot_commands>()) {
			Json::Value ObjectArray = ConfigSlips::ConfigJson["slashcommand"];
			std::cout << ObjectArray.size();
			int iter_1 = 1;
			for (int iter_0 = 0; iter_0 != ObjectArray.size(); ++++iter_0) {
				std::cout << ObjectArray[iter_0].asString() << ":" << ObjectArray[iter_1].asString() << std::endl;
				RobotSlips::bot->global_command_create(dpp::slashcommand(ObjectArray[iter_0].asString(), ObjectArray[iter_1].asString(), RobotSlips::bot->me.id));
				++++iter_1;
			}

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

			RobotSlips::bot->global_command_create(dpp::slashcommand("update", "プログラム更新の起動", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_string, "option", "更新作成"))
			);

			//语言识别
			RobotSlips::bot->global_bulk_command_create({ dpp::slashcommand("record", "Joins your voice channel and records you.", RobotSlips::bot->me.id) , dpp::slashcommand("stop", "Stops recording you.", RobotSlips::bot->me.id) });

		}//If End;

		});//END

	//
	Json::Value ObjectArray = ConfigSlips::ConfigJson["HashSlips"]["channl"];
	for (int iter = 0; iter < ObjectArray.size(); ++++ ++iter) {

		dpp::snowflake  channel = ObjectArray[iter + 1].asInt64();
		std::string To = ObjectArray[iter + 2].asString();

		(*HashSlips::HashSnowflakeStr)[ObjectArray[iter].asInt64()] = std::pair<dpp::snowflake, std::string>(channel, To);

		std::cout << ObjectArray[iter].asInt64() << ":" << channel << ":" << To << std::endl;
	}
}

void PlanPen::Slashcommand() {
	SlashcommandHash("翻訳の開始", [](dpp::slashcommand_t* event)->void {
		//将数据存入哈希表
		if ((*HashSlips::HashSnowflakeStr)[event->command.channel_id] == std::pair<dpp::snowflake, std::string>())
			event->reply("わかった");
		else
			event->reply("はい、リダイレクトチャンネル");

		//dpp::command_interaction cmd_data = event->command.get_command_interaction();

		dpp::snowflake  channel = std::get<dpp::snowflake>(event->get_parameter("翻至"));
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

	//update
	SlashcommandHash("update", [](dpp::slashcommand_t* event) -> void {

		LinuxPen::update(event);

		});

	RobotSlips::bot->on_slashcommand([](dpp::slashcommand_t event) {
		(*HashSlips::SlashcommandFuntion)[event.command.get_command_name()](&event);
		});

	SlashcommandHash("record", [](dpp::slashcommand_t* event)->void {



		/* Check which command they ran */
			/* Get the guild */
		dpp::guild* g = dpp::find_guild(event->command.guild_id);

		/* Attempt to connect to a voice channel, returns false if we fail to connect. */
		if (!g->connect_member_voice(event->command.get_issuing_user().id)) {
			event->reply("You don't seem to be in a voice channel!");
			return;
		}

		/* Tell the user we joined their channel. */
		event->reply("Joined your channel, now recording!");
		
		});

	SlashcommandHash("stop", [](dpp::slashcommand_t* event)->void {
		event->from->disconnect_voice(event->command.guild_id);
		//fclose(fd);

		event->reply("Stopped recording.");
		});

}//slashcommand end

//建立哈希索引
void PlanPen::SlashcommandHash(std::string command, void(*Funtion)(dpp::slashcommand_t*)) {
	(*HashSlips::SlashcommandFuntion)[command] = Funtion;

}

void PlanPen::AutoComplete() {
	RobotSlips::bot->on_autocomplete([](const dpp::autocomplete_t& event) {
		dpp::interaction_response AutoType(dpp::ir_autocomplete_reply);

		for (auto& opt : event.options) {
			if (opt.focused) {
				std::string uservalue = std::get<std::string>(opt.value);

				if (uservalue == "") {
					for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"].size(); ++i) {
						AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].asString(), ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["language"].asString()));
					}
					RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
					return;
				}

				//TODO
				for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"].size(); ++i) {
					if (ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].asString().find(uservalue) != -1)
						AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].asString(), ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["language"].asString()));
				}
				RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
			}
		}
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

		auto MessageObj = WebPen::TranslationPen(std::move(ss.str()), (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second)["translations"][0];


		dpp::embed ObjEmbed = dpp::embed()
			.set_description(event.msg.content + "\n[☯](" + event.msg.get_url() + ")")
			.set_color(dpp::colors::yellow)
			.set_author(event.msg.author.global_name, "", event.msg.author.get_avatar_url());


		ObjEmbed.add_field("", std::move(MessageObj["text"].asString()));

		ObjEmbed.set_footer(
			dpp::embed_footer()
			.set_text(std::move("⚝:>" + MessageObj["detected_source_language"].asString()))
		);

		ObjEmbed.set_timestamp(time(0));


		//create to object
		dpp::message TrText = dpp::message()
			.set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first)
			.add_embed(std::move(ObjEmbed));

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
			//???
			RobotSlips::bot->message_create(dpp::message(obj["url"].get<std::string>()).set_channel_id((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first));
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

//TODO:更新用户编辑消息
void PlanPen::MessageUpdate() {
	RobotSlips::bot->on_message_update([](const dpp::message_update_t event) {
		if ((*HashSlips::HashSnowflakeStr)[event.msg.id].first == 0 || event.msg.author.global_name == "")
			return;

		dpp::message msg(event.msg.author.global_name + ":" + WebPen::TranslationPen(event.msg.content, (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second)["translations"][0]["text"].asString());

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

	std::vector<std::string> RegexStr = {
		R"(https?://[^\s/$.?#].[^\s]*)"
		//R"(<:([^:]+):([^>]+)>)"
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


void PlanPen::ChannlConfigBookUpdate() {
	Json::Value Channl;
	for (auto Obj : (*HashSlips::HashSnowflakeStr)) {
		if (Obj.first == NULL)
			continue;
		else if (Obj.second.first == NULL)
			continue;
		else if (Obj.second.second == "")
			continue;

		Channl.append((uint64_t)Obj.first);
		Channl.append((uint64_t)Obj.second.first);
		Channl.append(Obj.second.second);
	}

	ConfigSlips::ConfigJson["HashSlips"]["channl"] = std::move(Channl);

	std::ofstream outFile(ConfigSlips::Path_, std::ofstream::trunc); // 使用trunc模式覆盖原文件
	if (!outFile.is_open()) {
		std::cerr << "Failed to open file for writing" << std::endl;
	}

	Json::StreamWriterBuilder writerBuilder;
	std::string jsonString = Json::writeString(writerBuilder, ConfigSlips::ConfigJson);
	outFile << jsonString;
	outFile.close();
}

std::string LinuxPen::cmd(const char* command) {
	char result[10240] = { 0 };
	char buf[10240] = { 0 };

	FILE* fp = NULL;
	if ((fp = popen(command, "r")) == NULL) {
		printf("popen error!\n");
		return "[error]";
	}
	while (fgets(buf, sizeof(buf), fp)) {
		strcat(result, buf);
	}

	return result;
}

void LinuxPen::update(dpp::slashcommand_t* event) {
	event->reply("更新を試みる");

	if (!32768 == system("cd ./MotooriKosuzu;git pull"))
		RobotPen::GetBot()->message_create(dpp::message("更新開始").set_channel_id(event->command.channel_id).set_guild_id(event->command.guild_id));
	else
		RobotPen::GetBot()->message_create(dpp::message("更新は存在しません\nプログラムの更新を試みる").set_channel_id(event->command.channel_id).set_guild_id(event->command.guild_id));

	if (!32768 == system("git clone https://github.com/touhou-technology/MotooriKosuzu"))
		RobotPen::GetBot()->message_create(dpp::message("Github倉庫のクローニング").set_channel_id(event->command.channel_id).set_guild_id(event->command.guild_id));

	RobotPen::GetBot()->message_create(dpp::message("再コンパイルの開始").set_channel_id(event->command.channel_id).set_guild_id(event->command.guild_id));

	//docker
	if (!32768 == system(R"(cd /root/MotooriKosuzu/src;g++ Application.cpp BambooSlips.h Bookshelf.hpp MotooriKosuzu.cpp MotooriKosuzu.h start.hpp WritingBrush.cpp WritingBrush.h -std=c++20 -l"dpp" -l"pthread" -l"jsoncpp" -l"curl" -o Project.out)")) {
		RobotPen::GetBot()->message_create(dpp::message("プログラムの再コンパイルが完了しました").set_channel_id(event->command.channel_id).set_guild_id(event->command.guild_id));
	}
	else
		RobotPen::GetBot()->message_create(dpp::message("いや、インクがひっくり返った").set_channel_id(event->command.channel_id).set_guild_id(event->command.guild_id));

	RobotSlips::bot.release();
}