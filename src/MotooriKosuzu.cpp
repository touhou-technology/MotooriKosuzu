#include "MotooriKosuzu.h"
#include "WritingBrush.h"
#include "Dictation.h"
#include "Stone.h"

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

Kosuzu& Kosuzu::WriteStone(){
	static StoneTranslationObj AwA;
}


Kosuzu& Kosuzu::Start() {
	RobotPen::Start();
}