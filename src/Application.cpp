#include "start.hpp"
#include "BambooSlips.h"

int main(int argc, char** argv) {
	Kosuzu::Init();

	//debug
	RobotSlips::bot->on_log(dpp::utility::cout_logger());

	Kosuzu::Start();

	return 9;
}