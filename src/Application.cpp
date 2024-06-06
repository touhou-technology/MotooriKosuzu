#include <iostream>
#include <memory>
#include "MotooriKosuzu.h"

using namespace std;

void Plan() {
	//test funtion

}

int main(){
	//start bot
	auto MK = make_unique<Kosuzu>();
	MK->Init();
	return 0;
}