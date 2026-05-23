module;

#include <dpp/cluster.h>
#include <iostream>

export module dppm:cluster;

export namespace dppm {
class api {
  public:
	void Test(int _);
};
}; // namespace dppm

// impl
namespace dppm {
void api::Test(int _) {
	std::cout << "Test" << std::endl;
	return;
}
} // namespace dppm
