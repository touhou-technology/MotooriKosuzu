#include "Voice.h"
#include "BambooSlips.h"


void InitVoice::Init() {
	S_TranslateVoiceConfig::Init();
}

void S_TranslateVoiceConfig::Init() {
	Slashcommand();
	Voice();
}  

void S_TranslateVoiceConfig::Slashcommand() {
	//ÓïÑÔÊ¶±ð
	RobotSlips::bot->global_command_create(dpp::slashcommand("record", "Join", RobotSlips::bot->me.id));
	RobotSlips::bot->global_command_create(dpp::slashcommand("stop", "Stops", RobotSlips::bot->me.id));
}



void S_TranslateVoiceConfig::Voice() {
	RobotSlips::bot->on_voice_ready([&](const dpp::voice_ready_t& event) {
		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), "voice_ready");

		});

	RobotSlips::bot->on_voice_receive([&](const auto& event) {
		RobotSlips::bot->log(dpp::loglevel(dpp::ll_debug), "voice_receive");
	

		});//end

}//PlanVoice::Voice END

TranslateVoice::TranslateVoice(const Specification& Spec)
	:m_Speci(Spec){

}