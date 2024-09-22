#include <iostream>
#include "start.hpp"

int main(int argc, char** argv) {
	Kosuzu::Init();

	Linux_Mailbox::reset_pid(argc);

	//Linux_Mailbox::send_msg("hello~");

	//debug
	RobotSlips::bot->on_log(dpp::utility::cout_logger());

	RobotPen::Start();

	while (Linux_Mailbox::GetRuning())
		sleep(5);

	return 9;
}