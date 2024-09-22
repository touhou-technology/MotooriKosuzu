#include <iostream>
#include "start.hpp"

int main(int argc, char** argv) {
	Kosuzu::Init();

	//debug
	RobotSlips::bot->on_log(dpp::utility::cout_logger());

	RobotPen::Start();

	Kosuzu::SetRuning(1);

	while (Kosuzu::GetRuning())
		sleep(5);

	return 9;
}