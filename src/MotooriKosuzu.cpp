#include "MotooriKosuzu.h"
#include "WritingBrush.h"
#include "Voice.h"

//类名就是包括该类的作用

//该函数为为类初始化
void Kosuzu::Init() {
	m_Kosuzu.reset(new Kosuzu());
}

Kosuzu::Kosuzu() {
	InitPen::Init();
	InitVoice::Init();
}

void Kosuzu::Start() {
	RobotPen::Start();
}