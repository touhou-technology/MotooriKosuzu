module;

#include <dpp/cluster.h>
#include <iostream>
#include <string>

export module dppm;

export namespace dppm {
class bot {
  public:
	bot(std::string token) : api(token) { return; };

	//  private:
	dpp::cluster api;
};
} // namespace dppm

export void TestDPP() {
	// dpp::cluster obj("");
	// obj.start(dpp::st_return);

	std::cout << "test" << std::endl;
}
