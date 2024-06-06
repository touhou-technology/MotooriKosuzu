#include <iostream>
#include "start.hpp"

void Plan() {
	//test funtion
	RobotPen::work([](dpp::cluster* bot)->void {
		bot->on_message_create([](dpp::message_create_t event) {
			if (event.msg.author.id != RobotPen::GetBot()->me.id)
				event.reply("awa");
			});

		});
}

#include <iostream>
int main() {
	// Kosuzu::Init("114514");
	Plan();

	//start bot
	RobotSlips::bot->on_log(dpp::utility::cout_logger());
	RobotSlips::bot->start(dpp::st_wait);
	return 9;
}