#include "MotooriKosuzu.h"

#include "WritingBrush.h"
#include "Bookshelf.hpp"

//类名就是包括该类的作用
using namespace MotooriKosuzu;

//该函数为为类初始化
void Kosuzu::Init() {
	m_Kosuzu.reset(new Kosuzu());
}

Kosuzu::Kosuzu()
//添加bot令牌
	:bot(Pen::ConfigPen::GetConfigJson().get("BotToken", "null").asString()) {
	Pen::ConfigPen::init();
}

Kosuzu::~Kosuzu() {

}
