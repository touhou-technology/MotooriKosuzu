#include "MotooriKosuzu.h"
#include "WritingBrush.h"

namespace MotooriKosuzu {
	//创建实例，因为原本为一个抽象类
	std::unique_ptr<Kosuzu> Kosuzu::m_Kosuzu = nullptr;

	void Init(std::string path) {
		Kosuzu::Init();
		std::cout << "\n" << Pen::ConfigPen::GetConfigJson().get("name","null").asString();
		std::cout << "\n" << Pen::ConfigPen::GetConfigJson().get("test","null").asString();
		std::cout << "\n" << Pen::ConfigPen::GetConfigJson().get("awa","null").asString();
		std::cout << "\n" << Pen::ConfigPen::GetConfigJson()["awa"];
		std::cout << "\n" << (Pen::ConfigPen::GetConfigJson()["major"][1])["computer"];
	}
}