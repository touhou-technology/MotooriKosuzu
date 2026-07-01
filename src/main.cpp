#include <algorithm>
#include <format>
#include <iostream>
#include <print>
#include <ranges>

#include "core/core.hpp"
// #include "core/reflection.hpp"
#include <meta>

// import dppm;
struct Test {
	void f() { std::println("Hello World"); };
	static void g();
};

void print_class_members() {
	static int i = 0;
	constexpr auto ctx = std::meta::access_context::unchecked();
	constexpr static auto members =
		std::define_static_array(std::meta::members_of(^^Test, ctx));
	template for (constexpr auto e : members) {
		;
		std::cout << i << std::endl;
	}
}

int main() {
	Test owo;

	constexpr auto rx = std::meta::members_of(
		^^Test, std::meta::access_context::unchecked())[0];
	//[:f2:](2);

	owo.[:rx:]();

	print_class_members();
	return 0;
}
