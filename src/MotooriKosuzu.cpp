#include "MotooriKosuzu.h"
#include "WritingBrush.h"
#include "Dictation.h"
#include "Stone.h"

std::unique_ptr<StoneTranslationObj> StoneTranslationObj::m_instance;

//×¢ÒâË³Ðò
Kosuzu::Kosuzu() {
	InitPen::Init();
	//InitVoice::Init();
}

Kosuzu& Kosuzu::StartDebug(){
	//debug
	RobotPen::StartDebug();

	return *this;
}

Kosuzu& Kosuzu::WriteStone(){
	StoneTranslationObj::m_instance.reset(new StoneTranslationObj);

	return *this;
}


Kosuzu& Kosuzu::Start() {
	RobotPen::Start();

	return *this;
}