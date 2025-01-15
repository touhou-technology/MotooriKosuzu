#include "Dictation.h"
#include "BambooSlips.h"
#include "WritingBrush.h"

void InitVoice::Init() {
	S_TranslateVoiceConfig::Init();
}

void S_TranslateVoiceConfig::Init() {
	Slashcommand();
	Voice();
	AutoComplete();
}

void S_TranslateVoiceConfig::Slashcommand() {
	UsePen::SlashcommandHash("音声入力開始", [](dpp::slashcommand_t* event)->void {
		dpp::guild* g = dpp::find_guild(event->command.guild_id);

		/* Attempt to connect to a voice channel, returns false if we fail to connect. */
		if (!g->connect_member_voice(event->command.get_issuing_user().id)) {
			event->reply("You don't seem to be in a voice channel!");
			return;
		}

		event->reply("Okey~");
		});//End

	UsePen::SlashcommandHash("音声入力終了", [](dpp::slashcommand_t* event)->void {
		event->from->disconnect_voice(event->command.guild_id);

		//VoiceSlips::S_TranslateVoice.reset();
		RobotSlips::bot->global_bulk_command_delete();


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

void TranslateVoice::AddUser(dpp::snowflake obj, user_params params){
	m_object[obj] = params;
}

void TranslateVoice::AddUser(dpp::snowflake obj, user_params&& params){
	m_object[obj] = std::move(params);
}

void TranslateVoice::DelUser(dpp::snowflake obj){
	fclose(m_object[obj].record);
	m_object.erase(obj);
}

//处理语言
void TranslateVoice::SendVC(const dpp::voice_receive_t& event){

}

void TranslateVoice::SetFlag(){
	flag = false;
}

void TranslateVoice::Understand() {
	for (auto obj: m_object) {

	}
}

TranslateVoice::TranslateVoice(){
	std::thread([&] {
		static auto a = 9;
		while (1) {
			Understand();
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		}).detach();//End
}