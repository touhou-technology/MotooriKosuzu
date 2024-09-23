#include <iostream>
#include "start.hpp"

int main(int argc, char** argv) {
	Kosuzu::Init();

	//debug
	RobotSlips::bot->on_log(dpp::utility::cout_logger());

	RobotPen::Start();

	return 9;
}