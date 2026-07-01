#include <algorithm>
#include <debugging>
#include <meta>
#include <optional>
#include <print>

bool Test_Debug = false;

void test() {
	std::cout << "Test Code" << std::endl;

	if (std::is_debugger_present()) {
		Test_Debug = true;
		std::cout << "Debug" << std::endl;
	}
	std::breakpoint_if_debugging();

	std::cout << "Continue\n";
}

int main(int argc, char *argv[]) {
	test();
	;

	return 0;
}
