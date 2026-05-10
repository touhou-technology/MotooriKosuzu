#include <iostream>
#include <print>

#include "core/core.hpp"

import DPPM;

int main() {
	UseAPI Test;

	auto Obj = Test.ReflectionAPI();
	for (auto Str : Obj) {
		std::cout << Str << std::endl;
	}

	TestDPP();
}
