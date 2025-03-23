#include "Stone.h"

StoneTranslationObj::StoneTranslationObj(){

}

void StoneTranslationObj::ChangeWrie(nlohmann::json& tmp){
	this->Write = tmp;
}
