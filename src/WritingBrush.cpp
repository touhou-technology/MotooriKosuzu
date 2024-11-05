#include "WritingBrush.h"
#include "BambooSlips.h"
#include "MotooriKosuzu.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <regex>

using namespace std;

void ConfigPen::Init() {
	ConfigSlips::ConfigJson = ReadFileJson(ConfigSlips::Path_);

	//TODO
}

//类Init御用（
std::string ConfigPen::InitPen(std::string ClassName, std::string obtain) {
	return ConfigSlips::ConfigJson[ClassName][obtain].asString();
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

//Json::Value ConfigPen::GetConfigJson() {
//	return ConfigSlips::ConfigJson;
//}

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
	if (text == "")
		return "";

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
	RobotSlips::bot->on_ready([](const dpp::ready_t event) {
		//RobotSlips::bot->global_bulk_command_delete();
		//(),需要添加新的解析
		if (dpp::run_once<struct register_bot_commands>()) {
			Json::Value ObjectArray;
			ObjectArray = ConfigSlips::ConfigJson["slashcommand"];
			std::cout << ObjectArray.size();
			int iter_2 = 1;
			for (int iter = 0; iter != ObjectArray.size(); ++++iter) {
				std::cout << ObjectArray[iter].asString() << ":" << ObjectArray[iter_2].asString() << std::endl;
				RobotSlips::bot->global_command_create(dpp::slashcommand(ObjectArray[iter].asString(), ObjectArray[iter_2].asString(), RobotSlips::bot->me.id));
				++++iter_2;
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

	//update
	SlashcommandHash("update", [](dpp::slashcommand_t* event) -> void {

		LinuxPen::update(event);

		});

	RobotSlips::bot->on_slashcommand([](dpp::slashcommand_t event) {
		(*HashSlips::SlashcommandFuntion)[event.command.get_command_name()](&event);
		});
}

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
		//std::cout << event.msg.to_json() << std::endl;

		//单向翻译监测是否有
		if ((*HashSlips::HashSnowflakeStr)[event.msg.channel_id].first == 0 || event.msg.author.id == RobotSlips::bot->me.id)
			return;

		//build object
		nlohmann::json data = event.msg.to_json();

		//create temp Text url
		std::string TextMsg = event.msg.content;

		//url做处理
		std::vector<std::string> Treatment = RegexTreatment(TextMsg);


		dpp::embed ObjEmbed = dpp::embed()
			.set_description(event.msg.content + "\n[☯](" + event.msg.get_url() + ")")
			.set_color(dpp::colors::yellow)
			//???
			.set_author(event.msg.author.global_name, "", event.msg.author.get_avatar_url());

		//处理字符串
		std::stringstream ss;
		for (char ch : TextMsg)
			if (ch == '"')
				ss << "\\\"";
			else if (ch == '\n')
				ss << "\\n";
			else
				ss << ch;

		//TODO：调用翻译
		ObjEmbed.add_field("", WebPen::TranslationPen(std::move(ss.str()), (*HashSlips::HashSnowflakeStr)[event.msg.channel_id].second)["translations"][0]["text"].asString());


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

	if (!32768 == system(R"(cd ./MotooriKosuzu/src;g++ Application.cpp BambooSlips.h Bookshelf.hpp MotooriKosuzu.cpp MotooriKosuzu.h start.hpp WritingBrush.cpp WritingBrush.h -std=c++20 -l"dpp" -l"pthread" -l"jsoncpp" -o Project.out)")) {
		RobotPen::GetBot()->message_create(dpp::message("プログラムの再コンパイルが完了しました").set_channel_id(event->command.channel_id).set_guild_id(event->command.guild_id));
	}
	else
		RobotPen::GetBot()->message_create(dpp::message("いや、インクがひっくり返った").set_channel_id(event->command.channel_id).set_guild_id(event->command.guild_id));

	RobotSlips::bot.release();
}