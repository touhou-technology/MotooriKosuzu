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
				dpp::slashcommand("VCに参加する", "Join VC", RobotSlips::bot->me.id)
			);

			RobotSlips::bot->global_command_create(
				dpp::slashcommand("音声入力開始", "同声传译(ASR)", RobotSlips::bot->me.id)
				.add_option(dpp::command_option(dpp::co_string, "language", "设置语言", true).set_auto_complete(true))
				.add_option(dpp::command_option(dpp::co_string, "model", "设置模型", true).set_auto_complete(true))
				.add_option(dpp::command_option(dpp::co_user, "id", "选择用户", true))
				.add_option(dpp::command_option(dpp::co_string, "time", "设置时间", true))
			);
			//other
			RobotSlips::bot->global_command_create(dpp::slashcommand("音声入力終了", "结束", RobotSlips::bot->me.id));
		}
		});
}

void S_TranslateVoiceConfig::Slashcommand() {
	UsePen::SlashcommandHash("VCに参加する", [](dpp::slashcommand_t* event) {
		dpp::guild* g = dpp::find_guild(event->command.guild_id);

		/* Attempt to connect to a voice channel, returns false if we fail to connect. */
		if (!g->connect_member_voice(event->command.get_issuing_user().id)) {
			event->reply("You don't seem to be in a voice channel!");
			return;
		}

		VoiceSlips::S_TranslateVoice.reset(new TranslateVoice());

		event->reply("Okey~");
		});

	UsePen::SlashcommandHash("音声入力開始", [](dpp::slashcommand_t* event)->void {


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
	params.vc_record = fopen(std::to_string(params.ID).c_str(), "wb");
	m_object[params.ID] = params;
}

void TranslateVoice::DelUser(dpp::snowflake obj) {
	fclose(m_object[obj].vc_record);
	m_object.erase(obj);
}

//处理语言
void TranslateVoice::SendVC(const dpp::voice_receive_t& event) {
	if (m_object[event.user_id].ID != event.user_id)
		return;

	fwrite((char*)event.audio, 1, event.audio_size, m_object[event.user_id].vc_record);
}

void TranslateVoice::ResetFlag() {

}

void TranslateVoice::Suitable() {
	for (auto object : m_object) {
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
	std::cout << "开始处理" << std::endl;
}

TranslateVoice::TranslateVoice() {
	a = std::thread([&] {
		while (1) {
			Suitable();
			std::this_thread::sleep_for(time);
		}

		});//End
	a.detach();
}

void TranslateVoice::test_add_instance() {
	TranslateVoice::user_params a;
	VoiceSlips::S_TranslateVoice->AddUser(a);
}