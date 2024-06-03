#include "BambooSlips.h"

using namespace BambooSlips;

void ConfigSlips::Init() {
	std::cout << "Slips test";
	str = "awa";
	std::cout << str;

}

std::string WebSlips::GetTranslationURL() {
	//return WebSlips::StrTranslationURL;
}

//Set class type
void WebSlips::SetURL(std::string Content, unsigned int Type) {
	switch (Type)
	{
	case 1:
		StrTranslationURL;
		break;
	case 2:

		break;
	default:
		std::cout << "没有设置任何类型" << std::endl;
		break;
	}
}
