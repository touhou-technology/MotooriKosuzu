#pragma once
#include <dpp/dpp.h>

#include <iostream>
#include <memory>
/*
自定义
*/

//最后交给这个类来进行初始化（也就是app本体？）
namespace MotooriKosuzu {

	class Kosuzu {
	public:
		static void Init();
		Kosuzu();
		~Kosuzu();
	private:
		static std::unique_ptr<Kosuzu> m_Kosuzu;
	};
}