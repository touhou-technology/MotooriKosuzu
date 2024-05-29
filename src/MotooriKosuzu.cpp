#include "MotooriKosuze.h"

MK::MotooriKosuze::MotooriKosuze(){
	m_robot = new dpp::activity();
}

//等等还没设计好
dpp::activity* MK::MotooriKosuze::GetM_robot() {
	return m_robot;
}