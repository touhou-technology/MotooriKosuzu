#include "MotooriKosuzu.h"

#include "WritingBrush.h"
#include "Bookshelf.hpp"

//类名就是包括该类的作用
using namespace MotooriKosuzu;

//该函数为为类初始化
void Kosuzu::Init() {
	m_Kosuzu.reset(new Kosuzu());
}

Kosuzu::Kosuzu() {
	//应该最先初始化，因为其他依赖于这个
	Pen::ConfigPen::Init();
	Pen::
	Pen::WebPen::Init();
}

Kosuzu::~Kosuzu() {

}
