#include "Ru_koto.h"

namespace Rult {
	void Ru_koto::Init(const std::string config) {
		instance_.reset(new Ru_koto(config));
	}

	void Ru_koto::Quit() {
		instance_.reset();
	}

	Ru_koto& Ru_koto::GetInstance() {
		return *instance_;
	}

	Ru_koto::Ru_koto(const std::string config)
		:ru_koto(/*TOKEN*/"OwO", dpp::i_default_intents | dpp::i_message_content), config_(config) {
		//logh

		ru_koto.on_log(dpp::utility::cout_logger());

		//Fn Init is here 
		//slashcommand();
		//voice();
		//monitor_message();
		//terminal();
		//interpreter();
		//QDSand();
		ru_koto.start(dpp::st_wait);
	}

	Ru_koto::~Ru_koto() {

	}
}