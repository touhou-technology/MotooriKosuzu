#pragma once

#include <iostream>
#include <memory>
#include <dpp/dpp.h>

#include <json/json.h>

//最后交给这个类来进行初始化（也就是app本体？）
	//本体daze
class Kosuzu {
	private:
		std::string config_dir;
		std::string local_dir;
		dpp::cluster kosuzu_bot;
	public:
		Kosuzu();
		~Kosuzu();

		void Init();
};
