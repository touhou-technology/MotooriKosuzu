#pragma once
#include <dpp/dpp.h>
#include <json/json.h>
#include <whisper.h>

#include "common.h"
#include <whisper.h>

class InitVoice {
public:
	static void Init();
};

class S_TranslateVoiceConfig {
public:
	static void Init();


	static void Slashcommand();
	static void Voice();
};

class TranslateVoice {
public:
	struct Specification {
		std::string language;
		std::string model;
	};

	TranslateVoice(const Specification& Spec);

private:
	Specification m_Speci;
};