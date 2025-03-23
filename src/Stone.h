//取自"把字刻在石头上"(笑)
#pragma once
#include <dpp/dpp.h>

#include "BambooSlips.h"
#include "WritingBrush.h"

class StoneTranslationObj {
public:
	StoneTranslationObj();
	
	~StoneTranslationObj() = default;
public:
	void ChangeWrie(nlohmann::json& tmp);

private:
	nlohmann::json Write = ConfigSlips::ConfigJson;
};