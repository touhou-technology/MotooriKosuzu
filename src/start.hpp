#include "MotooriKosuzu.h"
#include "WritingBrush.h"

	//创建实例，因为原本为一个抽象类
std::unique_ptr<Kosuzu> Kosuzu::m_Kosuzu = nullptr;

void Init(std::string path) {
	Kosuzu::Init();
	std::cout << "\n" << ConfigPen::GetConfigJson().get("name","null").asString();
	std::cout << "\n" << ConfigPen::GetConfigJson().get("test","null").asString();
	std::cout << "\n" << ConfigPen::GetConfigJson().get("awa","null").asString();
	std::cout << "\n" << ConfigPen::GetConfigJson()["awa"];
	std::cout << "\n" << (ConfigPen::GetConfigJson()["major"][1])["computer"];
}
