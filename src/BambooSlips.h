#pragma once
#include <iostream>

namespace BambooSlips {
	class ConfigSlips {
	public:
		static void Init();
	private:
		static std::string str;
	};
}