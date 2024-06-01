#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <regex>

/*
该类主要设计成为多个框架提供传递的方法
目前来说是这样的
*/
namespace RuKoto {
	class CoutContent;
	class UniversalContent;
	class Config;
	class Message;
	class Command;
	class JSregex;
}


class RuKoto::UniversalContent {
public:
	UniversalContent* get_m_UniversalContent();
private:
	RuKoto::UniversalContent* m_UniversalContent;
};

class RuKoto::Config {
public:
	void SetUniversalContent(UniversalContent* m_class);
private:
	RuKoto::UniversalContent* m_UniversalContent;
};