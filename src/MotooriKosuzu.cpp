#include "MotooriKosuzu.h"

#include "WritingBrush.h"
#include "Bookshelf.hpp"

//类名就是包括该类的作用
using namespace MotooriKosuzu;

//该函数为为类初始化
void Kosuzu::Init() {
	m_Kosuzu.reset(new Kosuzu());
}

void Kosuzu::work(void(*Fn)(dpp::cluster* bot)) {
	Fn(&bot);
}

Kosuzu::Kosuzu() {
	bot.on_log(dpp::utility::cout_logger());

	bot.start(dpp::st_wait);
}

Kosuzu::~Kosuzu() {

}
