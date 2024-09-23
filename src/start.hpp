#include "MotooriKosuzu.h"
#include "WritingBrush.h"
#include "BambooSlips.h"

	//创建实例，因为原本为一个抽象类
std::unique_ptr<Kosuzu> Kosuzu::m_Kosuzu = nullptr;
//设置运行状态

void Init(std::string path) {
	Kosuzu::Init();
}