#include <iostream>
#include <print>

#include "core/core.hpp"

import dppm;

int main() {
	UseAPI Test;
	// module test
	dppm::bot test;
	test.Test(1);

	auto Obj = Test.ReflectionAPI();
	for (auto Str : Obj) {
		std::cout << Str << std::endl;
	}
}
