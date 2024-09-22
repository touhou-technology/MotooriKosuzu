#include "MotooriKosuzu.h"

#include "WritingBrush.h"
#include "Bookshelf.hpp"

//类名就是包括该类的作用

//该函数为为类初始化
void Kosuzu::Init() {
	m_Kosuzu.reset(new Kosuzu());
}

Kosuzu::Kosuzu() {
	//应该最先初始化，因为其他依赖于这个
	ConfigPen::Init();
	HashPen::Init();

	WebPen::Init();
	//先要写什么再写什么
	RobotPen::Init();
	PlanPen::Init();
}

Kosuzu::~Kosuzu() {

}

void Kosuzu::SetRuning(bool run){
	runing = run;
}

bool Kosuzu::GetRuning(){
	return runing;
}
