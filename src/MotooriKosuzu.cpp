#include "MotooriKosuzu.h"
//给小铃拿起🖊
#include "WritingBrush.h"

//类名就是包括该类的作用
using namespace MotooriKosuzu;

//该函数为为类初始化
void Kosuze::Init() {
	m_Kosuze.reset(new Kosuze());
}

Kosuze::Kosuze() {
	std::cout << "TEST";
}

Kosuze::~Kosuze() {

}
