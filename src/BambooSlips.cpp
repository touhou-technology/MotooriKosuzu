#include "BambooSlips.h"

using namespace BambooSlips;

void ConfigSlips::Init() {
	std::cout << "Slips test";
}

std::string WebSlips::GetTranslationURL() {
	//return WebSlips::TranslationURL;
}

void WebSlips::SetURL(std::string Content, unsigned int Type) {
	switch (Type)
	{
	case 1:
		TranslationURL = Content;
	default:
		std::cout << "没有设置任何类型" << std::endl;
		break;
	}
}
