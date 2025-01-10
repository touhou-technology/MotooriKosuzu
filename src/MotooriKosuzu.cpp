#include "MotooriKosuzu.h"
#include "WritingBrush.h"
#include "Dictation.h"

//×¢ÒâË³Ðò
Kosuzu::Kosuzu() {
	InitPen::Init();
	InitVoice::Init();
}

Kosuzu& Kosuzu::StartDebug(){
	//debug
	RobotPen::StartDebug();

	return *this;
}

void Kosuzu::Start() {
	RobotPen::Start();
}