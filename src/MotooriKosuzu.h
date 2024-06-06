#pragma once

#include <iostream>
#include <memory>
#include <dpp/dpp.h>
#include <json/json.h>

//最后交给这个类来进行初始化（也就是app本体？）
//本体daze
class Kosuzu{
	private:
		std::string config_dir;
		std::string local_dir;
		unique_ptr<Json::Value> config;
		unique_ptr<dpp::cluster> kosuzu_bot;

		void Read_config();
	public:
		Kosuzu();
		~Kosuzu();

		void Init();
		void Reset();
};
