#include "MotooriKosuzu.h"
#include "WritingBrush.h"

namespace MotooriKosuzu {
	//创建实例，因为原本为一个抽象类
	std::unique_ptr<Kosuzu> Kosuzu::m_Kosuzu = nullptr;

	void Init(std::string path) {
		Kosuzu::Init();

	}
}