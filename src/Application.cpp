#include <iostream>
#include "start.hpp"

void Plan() {
	//echo
	RobotPen::work([](dpp::cluster* bot)->void {
		bot->on_message_create([](dpp::message_create_t event) {
			if (event.msg.author.id != RobotPen::GetBot()->me.id)
				event.reply(event.msg.content);
			});

		});
}

#include <iostream>
int main() {
	Kosuzu::Init();
	Plan();

	//debug
	RobotSlips::bot->on_log(dpp::utility::cout_logger());
	RobotPen::Start();
	return 9;
}