#include "MotooriKosuzu.h"
#include "WritingBrush.h"

Kosuzu::Kosuzu() {
	InitPen::Init	();
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