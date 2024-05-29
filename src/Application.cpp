#include "MotooriKosuze.h"
//需要预先写好配置文件
void MotooriKosuze_Init(const std::string Pair) {
	MotooriKosuze::DiscordAPI::Init();

}

//main作为主程序接口，其功能在类实现为多线程
int main() {
	std::cout << "该版本为最终不测试版本" << std::endl;
	MotooriKosuze_Init("~/server/config");
	return 0;
}