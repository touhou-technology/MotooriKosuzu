#include "start.hpp"

void Plan() {
	//test funtion
	Pen::RobotPen::work([](dpp::cluster* bot)->void {
		bot->on_message_create([](dpp::message_create_t event) {
			if (event.msg.author.id != Pen::RobotPen::GetBot()->me.id)
				event.reply("awa");
			});

		});
}

#include <iostream>
int main() {
	MotooriKosuzu::Init("114514");
	Plan();

	//start bot
	BambooSlips::RobotSlips::bot->on_log(dpp::utility::cout_logger());
	BambooSlips::RobotSlips::bot->start(dpp::st_wait);
	return 9;
}