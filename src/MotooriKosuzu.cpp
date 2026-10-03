#include "MotooriKosuzu.h"
// #include "Dictation.h"
#include "Stone.h"
#include "WritingBrush.h"

Kosuzu::Kosuzu() { InitPen::Init(); }

Kosuzu &Kosuzu::StartDebug() {
	RobotPen::StartDebug();

	return *this;
}

Kosuzu &Kosuzu::WriteStone() {
	StoneTranslationObj::m_instance.reset(new StoneTranslationObj);

	return *this;
}

Kosuzu &Kosuzu::Start() {
	RobotPen::Start();

	return *this;
}
