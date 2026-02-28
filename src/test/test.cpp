#include <algorithm>
#include <meta>
#include <print>

#include "core/reflection.hpp"

struct R;
consteval {
	make_named_tuple(^^R, {
							  {^^int, "Test_1"}, {^^double, "Test_2"}});
}

int main(int argc, char *argv[]) {
	auto A = R{.Test_1 = 1, .Test_2 = 2.1};
	auto &[T1, T2] = A;
	std::println("{}:{}", T1, T2);
	return 0;
}
