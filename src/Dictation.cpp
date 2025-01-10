#include "Dictation.h"
#include "BambooSlips.h"
#include "WritingBrush.h"

void InitVoice::Init() {
	S_TranslateVoiceConfig::Init();
}

void TranslateVoice::send(const dpp::voice_receive_t& event) {
	//std::move(event.audio_data);
	handle.resume();
}

char* TranslateVoice::get() {
	return handle.promise().return_data_;
}

void TranslateVoice::reset_handle(std::coroutine_handle<promise_type> new_handle) {
	//std::lock_guard<std::mutex> lock(handle_mutex); 
	if (handle) { handle.destroy(); } 
	handle = new_handle;
}

void TranslateVoice::test_move_next() {
	if (handle && !handle.done()) { handle.resume(); }
	else { std::cerr << "Invalid coroutine handle or coroutine already done." << std::endl; }
}

TranslateVoice::Specification& TranslateVoice::Specification_Reset() {
	return m_Speci;
}

void TranslateVoice::Specification_Reset(Specification param) {
	m_Speci = param;
}


void S_TranslateVoiceConfig::Init() {
	Slashcommand();
	Voice();
	AutoComplete();
}

TranslateVoice A() {
	co_yield "1";
	co_yield "2";
	co_yield "3";
	co_return "0";
}

void S_TranslateVoiceConfig::Slashcommand() {
	//语言识别
	RobotSlips::bot->on_ready([](const dpp::ready_t event) {
		RobotSlips::bot->global_command_create(
			dpp::slashcommand("音声入力開始", "加入语言并翻译", RobotSlips::bot->me.id)
			.add_option(dpp::command_option(dpp::co_string, "language", "", true).set_auto_complete(true))
			.add_option(dpp::command_option(dpp::co_string, "model", "", true).set_auto_complete(true))
			.add_option(dpp::command_option(dpp::co_user, "ID", "", true))
		);
		RobotSlips::bot->global_command_create(dpp::slashcommand("音声入力終了", "结束", RobotSlips::bot->me.id));
		//other
		RobotSlips::bot->global_command_create(dpp::slashcommand("launch", "Launch Realms of Wumpus", RobotSlips::bot->me.id));
		});//End

	UsePen::SlashcommandHash("音声入力開始", [](dpp::slashcommand_t* event)->void {
		dpp::guild* g = dpp::find_guild(event->command.guild_id);

		/* Attempt to connect to a voice channel, returns false if we fail to connect. */
		if (!g->connect_member_voice(event->command.get_issuing_user().id)) {
			event->reply("You don't seem to be in a voice channel!");
			return;
		}

		VoiceSlips::S_TranslateVoice.reset(std::move(&TranslateVoice(A())));

		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), VoiceSlips::S_TranslateVoice->get());
		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), VoiceSlips::S_TranslateVoice->get());

		VoiceSlips::S_TranslateVoice->test_move_next();

		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), VoiceSlips::S_TranslateVoice->get());



		event->reply("Okey~");
		});//End

	UsePen::SlashcommandHash("音声入力終了", [](dpp::slashcommand_t* event)->void {
		event->from->disconnect_voice(event->command.guild_id);

		//VoiceSlips::S_TranslateVoice.reset();


		event->reply("Okey~");
		});//End

}//End Slahcommd

void S_TranslateVoiceConfig::Voice() {
	RobotSlips::bot->on_voice_ready([&](const dpp::voice_ready_t& event) {
		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), "voice_ready");
		RobotSlips::bot->message_create(dpp::message("voice_ready").set_channel_id(event.voice_channel_id));

		});

	RobotSlips::bot->on_voice_receive([&](const dpp::voice_receive_t& event) {

		VoiceSlips::S_TranslateVoice->send(event);

		});//end

}
void S_TranslateVoiceConfig::AutoComplete() {
	RobotSlips::bot->on_autocomplete([](const dpp::autocomplete_t& event) {
		if (event.name != "音声入力開始")
			return;

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
		});//End
}
//PlanVoice::Voice END