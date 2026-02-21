#include <iostream>
#include <meta>


// test
int main() {
	struct S {
		int m;
		int n;
	};

	constexpr auto ctx = std::meta::access_context::current();
	template for (constexpr auto m : std::define_static_array(
					  nonstatic_data_members_of(^^S, ctx))) {
		std::cout << identifier_of(m) << std::endl;
	}
}
