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

g++ Application.cpp BambooSlips.h Bookshelf.hpp MotooriKosuzu.cpp MotooriKosuzu.h start.hpp WritingBrush.cpp WritingBrush.h -std=c++20 -l"dpp" -l"ssl" -l"pthread" -l"jsoncpp" -l"curl"