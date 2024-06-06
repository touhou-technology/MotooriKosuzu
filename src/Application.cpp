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
int main(){
	//start bot
	Kosuzu MK();
	return 0;
}