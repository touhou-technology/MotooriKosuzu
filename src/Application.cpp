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
	bool init_success = MK->Init();
	cout << "Initialized: " << init_success << endl;
	return 0;
}