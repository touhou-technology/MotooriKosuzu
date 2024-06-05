#include "MotooriKosuzu.h"

#include "WritingBrush.h"
#include "Bookshelf.hpp"

//类名就是包括该类的作用
using namespace MotooriKosuzu;

//该函数为为类初始化
void Kosuzu::Init() {
	std::cout << Pen::ConfigPen::GetConfigJson()["DiscordBotToken"] << std::endl;
	Token = Pen::ConfigPen::GetConfigJson().get("DiscordBotToken", "null").asString();
	m_Kosuzu.reset(new Kosuzu(Token));
}

Kosuzu::Kosuzu(std::string Token)
//添加bot令牌
	:bot(Token) {
	Pen::ConfigPen::init();
	bot.on_log(dpp::utility::cout_logger());
	bot.start(dpp::st_return);
}

Kosuzu::~Kosuzu() {

}
