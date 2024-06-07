#include <iostream>
#include "start.hpp"

int main() {
	Kosuzu::Init();

	//debug
	RobotSlips::bot->on_log(dpp::utility::cout_logger());
	RobotPen::Start();
	return 9;
}