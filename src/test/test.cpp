#include <algorithm>
#include <meta>
#include <print>

#include "core/reflection.hpp"

struct R;
consteval {
	mkr::make_named_tuple(^^R, {
								   {^^int, "Test_1"}, {^^double, "Test_2"}});
}

template <typename T, size_t N>
using struct_of_arrays = mkr::struct_of_arrays_impl<T, N>::impl;

struct point {
	float x;
	float y;
	float z;
};

int main(int argc, char *argv[]) {
	auto A = R{.Test_1 = 1, .Test_2 = 2.1};
	auto &[T1, T2] = A;
	std::println("{}:{}", T1, T2);

	using points = struct_of_arrays<point, 2>;

	points p = {.x = {1.1, 2.2}, .y = {3.3, 4.4}, .z = {5.5, 6.6}};

	for (size_t i = 0; i != 2; ++i) {
		std::println("({}, {}, {})", p.x[i], p.y[i], p.z[i]);
	}
	return 0;
}
