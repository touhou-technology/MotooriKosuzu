#include "MotooriKosuzu.h"

#include "WritingBrush.h"
#include "Desk.hpp"

//类名就是包括该类的作用
using namespace MotooriKosuzu;

//该函数为为类初始化
void Kosuzu::Init() {
	m_Kosuzu.reset(new Kosuzu());
}

Kosuzu::Kosuzu() {
	Pen::ConfigPen::init();
	std::cout << "TEST";
}

Kosuzu::~Kosuzu() {
	
}
