#include "Dictation.h"
#include "BambooSlips.h"
#include "WritingBrush.h"

void InitVoice::Init() {
	S_TranslateVoiceConfig::Init();
}

void S_TranslateVoiceConfig::Init() {
	OnReady();
	Slashcommand();
	Voice();
	AutoComplete();
}

void S_TranslateVoiceConfig::OnReady() {
	RobotSlips::bot->on_ready([&](dpp::ready_t event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			RobotSlips::bot->global_command_create(
				dpp::slashcommand("vcに参加する", "Join VC", RobotSlips::bot->me.id)
			);

			RobotSlips::bot->global_command_create(
				dpp::slashcommand("音声入力開始", "同声传译(ASR)", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_string, "language", "设置语言", true).set_auto_complete(true))
				.add_option(dpp::command_option(dpp::co_string, "model", "设置模型", true).set_auto_complete(true))
				.add_option(dpp::command_option(dpp::co_user, "id", "选择用户(Obj)", true))
				.add_option(dpp::command_option(dpp::co_channel, "channel", "选择输出(Obj)", true))
				.add_option(dpp::command_option(dpp::co_number, "time", "设置时间(milliseconds)"))
			);
			//other
			RobotSlips::bot->global_command_create(dpp::slashcommand("音声入力終了", "结束", RobotSlips::bot->me.id));
		}
		});
}

void S_TranslateVoiceConfig::Slashcommand() {
	UsePen::SlashcommandHash("vcに参加する", [](dpp::slashcommand_t* event) {
		dpp::guild* g = dpp::find_guild(event->command.guild_id);

		/* Attempt to connect to a voice channel, returns false if we fail to connect. */
		if (!g->connect_member_voice(event->command.get_issuing_user().id)) {
			event->reply("You don't seem to be in a voice channel!");
			return;
		}



		VoiceSlips::S_TranslateVoice.reset();
		VoiceSlips::S_TranslateVoice.reset(new TranslateVoice());
		event->reply("Okey~");
		});

	UsePen::SlashcommandHash("音声入力開始", [](dpp::slashcommand_t* event)->void {
		if (VoiceSlips::S_TranslateVoice == nullptr) {
			event->reply("未找到实例可能是没有加入VC");
			return;
		};

		//std::cout << event->raw_event << std::endl;
		TranslateVoice::user_params params;

		for (auto obj : std::get<dpp::command_interaction>(event->command.data).options) {

			std::cout << obj.name << std::endl;

			if (obj.name == "language") {
				params.language = std::get<std::string>(obj.value);
			}

			if (obj.name == "model") {
				params.model = std::get<std::string>(obj.value);
			}

			if (obj.name == "id") {
				params.id = std::get<dpp::snowflake>(obj.value);
			}

			if (obj.name == "channel") {
				VoiceSlips::S_TranslateVoice->SetCout(std::get<dpp::snowflake>(obj.value));
			}

			if (obj.name == "time") {
				//TODO
				//params.time = std::chrono::milliseconds(std::get<int>(obj.value));
			}
		}//for End

		params.name = event->command.usr.global_name;

		VoiceSlips::S_TranslateVoice->AddUser(std::move(params));

		event->reply("OwO");

		});//End

	UsePen::SlashcommandHash("音声入力終了", [](dpp::slashcommand_t* event)->void {
		event->from->disconnect_voice(event->command.guild_id);

		//RobotSlips::bot->global_bulk_command_delete();

		VoiceSlips::S_TranslateVoice.reset();

		event->reply("Okey~");
		});//End
}//End Slahcommd

void S_TranslateVoiceConfig::Voice() {
	RobotSlips::bot->on_voice_ready([&](const dpp::voice_ready_t& event) {
		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), "voice_ready");
		RobotSlips::bot->message_create(dpp::message("voice_ready").set_channel_id(event.voice_channel_id));

		});

	RobotSlips::bot->on_voice_receive([&](const dpp::voice_receive_t& event) {
		VoiceSlips::S_TranslateVoice->SendVC(event);
		});//end

}
void S_TranslateVoiceConfig::AutoComplete() {
	RobotSlips::bot->on_autocomplete([](const dpp::autocomplete_t& event) {
		if (event.name != "音声入力開始")
			return;

		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), event.name);

		dpp::interaction_response AutoType(dpp::ir_autocomplete_reply);

		//std::cout << event.raw_event << std::endl;

		for (auto& opt : event.options) {
			std::cout << opt.focused << std::endl;

			//language
			if (opt.name == "language" && opt.focused) {
				std::string uservalue = std::get<std::string>(opt.value);

				if (uservalue == "") {
					for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"].size(); ++i) {
						AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].get<std::string>(), ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["language"].get<std::string>()));
					}
					RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
					return;
				}

				//
				for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"].size(); ++i) {
					if (ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].get<std::string>().find(uservalue) != -1)
						AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["name"].get<std::string>(), ConfigSlips::ConfigJson["AutoComplete"]["TranslationTypes"][i]["language"].get<std::string>()));
				}
				RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
			}

			//model
			if (opt.name == "model" && opt.focused) {
				std::string uservalue = std::get<std::string>(opt.value);

				if (uservalue == "") {
					for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["Model"].size(); ++i) {
						AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["Model"][i]["name"].get<std::string>(), ConfigSlips::ConfigJson["AutoComplete"]["Model"][i]["model"].get<std::string>()));
					}
					RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
					return;
				}

				for (int i = 0; i != ConfigSlips::ConfigJson["AutoComplete"]["Model"].size(); ++i) {
					if (ConfigSlips::ConfigJson["AutoComplete"]["Model"][i]["name"].get<std::string>().find(uservalue) != -1)
						AutoType.add_autocomplete_choice(dpp::command_option_choice(ConfigSlips::ConfigJson["AutoComplete"]["Model"][i]["name"].get<std::string>(), ConfigSlips::ConfigJson["AutoComplete"]["Model"][i]["model"].get<std::string>()));
				}
				RobotSlips::bot->interaction_response_create(event.command.id, event.command.token, AutoType);
			}
		}
		});//End
}
//PlanVoice::Voice END

void TranslateVoice::AddUser(user_params params) {
	params.vc_record = fopen((std::to_string(params.id) + ".pcm").c_str(), "wb");
	m_object[params.id] = params;
}

void TranslateVoice::DelUser(dpp::snowflake obj) {
	fclose(m_object[obj].vc_record);
	m_object.erase(obj);
}

void TranslateVoice::SetCout(dpp::snowflake obj) {
	ObjCout = obj;
}

//处理语言
void TranslateVoice::SendVC(const dpp::voice_receive_t& event) {
	if (VoiceSlips::S_TranslateVoice == nullptr) {
		return;
	}
	//开始的时候用户数值是0
	if (m_object[event.user_id].id != event.user_id || (&m_object[event.user_id]) == nullptr || event.user_id == 0)
		return;

	VoiceSlips::S_TranslateVoice->ResetFlag();

	fwrite((char*)event.audio, 1, event.audio_size, m_object[event.user_id].vc_record);
}

void TranslateVoice::ResetFlag() {
	for (auto& object : m_object) {
		object.second.flag = std::chrono::milliseconds(0);
	}

	flag = true;
}

void TranslateVoice::Suitable() {

	//std::cout << "ping" << std::endl;

	for (auto& object : m_object) {
		object.second.flag += time;
		if (object.second.flag < object.second.time) {
			return;
		}

		object.second.flag = std::chrono::milliseconds(0);

		std::thread([&] {
			Understand(object.second);
			}).detach();
	}
}

void TranslateVoice::Understand(user_params& user) {
	//debug
	if (flag) {
		flag = false;
	}
	else {
		return;
	}

	std::string FileName = std::to_string(user.id);

	fclose(user.vc_record);
	system(("mv ./" + FileName + ".pcm " + FileName + "tmp.pcm").c_str());
	user.vc_record = fopen((std::to_string(user.id) + ".pcm").c_str(), "wb");

	system(("ffmpeg -f s16le -ar 96000 -i ./" + FileName + +"tmp.pcm -ac 2 -ar 16000 " + FileName + ".wav -y").c_str());

	loadModel(user.model);

	//转小写处理
	std::string language = user.language;

	for (auto it = language.begin(); it != language.end(); it++) { 
		*it = tolower(*it); 
	}

	std::cout << language << std::endl;

	std::string ASR = performInference(("./" + FileName + ".wav").c_str(), language)["text"].get<std::string>();

	if (ASR == "") {
		return;
	}

	std::cout << ASR << std::endl;

	RobotSlips::bot->message_create(dpp::message(user.name + ":" + ASR).set_channel_id(ObjCout));
}

size_t TranslateVoice::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void TranslateVoice::loadModel(const std::string& modelPath) {
	system((R"(curl 127.0.0.1:8080/load -H "Content-Type: multipart/form-data" -F model=")" + modelPath + "\"").c_str());
}


nlohmann::json TranslateVoice::performInference(const std::string& filePath, const std::string& language) {
	CURL* curl;
	CURLcode res;
	curl_mime* form = nullptr;
	curl_mimepart* field = nullptr;
	std::string readBuffer;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		form = curl_mime_init(curl);

		field = curl_mime_addpart(form);
		curl_mime_name(field, "file");
		curl_mime_filedata(field, filePath.c_str());

		field = curl_mime_addpart(form);
		curl_mime_name(field, "temperature");
		curl_mime_data(field, "0.0", CURL_ZERO_TERMINATED);

		field = curl_mime_addpart(form);
		curl_mime_name(field, "temperature_inc");
		curl_mime_data(field, "0.2", CURL_ZERO_TERMINATED);

		field = curl_mime_addpart(form);
		curl_mime_name(field, "response_format");
		curl_mime_data(field, "json", CURL_ZERO_TERMINATED);

		field = curl_mime_addpart(form);
		curl_mime_name(field, "language");
		curl_mime_data(field, "zh", CURL_ZERO_TERMINATED);

		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/inference");
		curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}

		curl_mime_free(form);
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	// Parse the JSON response
	nlohmann::json jsonResponse = nlohmann::json::parse(readBuffer);
	return jsonResponse;
}

TranslateVoice::TranslateVoice() {
	std::thread([&] {

		std::this_thread::sleep_for(std::chrono::seconds(3));

		while (1) {
			if (VoiceSlips::S_TranslateVoice == nullptr) {
				std::cout << "终止" << std::endl;
				return;
			}

			Suitable();
			std::this_thread::sleep_for(time);
		}

		}).detach();
}