#pragma once
#include <dpp/dpp.h>

class InitVoice {
public:
	static void Init();
};

class S_TranslateVoiceConfig {
public:
	static void Init();

private:
	static void Slashcommand();
	static void Voice();
	static void AutoComplete();
};