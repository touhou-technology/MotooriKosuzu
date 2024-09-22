#pragma once
#include <dpp/dpp.h>

#include <iostream>
#include <memory>
//最后交给这个类来进行初始化（也就是app本体？）
	//本体daze
class Kosuzu {
	public:
		static void Init();
		Kosuzu();
		~Kosuzu();

		static void SetRuning(bool run);
		static bool GetRuning();
	private:
		static std::unique_ptr<Kosuzu> m_Kosuzu;
		static bool runing;
};
