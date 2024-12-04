#pragma once
#include <dpp/dpp.h>

#include <iostream>
#include <memory>
//最后交给这个类来进行初始化（也就是app本体？）
	//本体daze
class Kosuzu {
public:
	static void Init();
	static void Start();
	Kosuzu();

private:
	static std::unique_ptr<Kosuzu> m_Kosuzu;

};
