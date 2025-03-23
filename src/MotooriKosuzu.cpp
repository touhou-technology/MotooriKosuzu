#include "MotooriKosuzu.h"
#include "WritingBrush.h"
#include "Dictation.h"

//ע��˳��
Kosuzu::Kosuzu() {
	InitPen::Init();
	InitVoice::Init();
}

Kosuzu& Kosuzu::StartDebug(){
	//debug
	RobotPen::StartDebug();

	return *this;
}

Kosuzu& Kosuzu::Start() {
	RobotPen::Start();
}