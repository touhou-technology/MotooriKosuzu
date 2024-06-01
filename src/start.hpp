#include "MotooriKosuzu.h"

namespace MotooriKosuzu {
	//创建实例，因为原本为一个抽象类
	std::unique_ptr<Kosuze> Kosuze::m_Kosuze = nullptr;

	void Init(std::string path) {
		Kosuze::Init();
	}
}