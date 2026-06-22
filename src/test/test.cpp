#include <algorithm>
#include <debugging>
#include <meta>
#include <optional>
#include <print>
#include <ranges>

#include "core/reflection.hpp"

#define SIMDJSON_STATIC_REFLECTION 1
#include "simdjson.h"

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

struct R;
consteval {
	mkr::make_named_tuple(^^R, {
								   {^^int, "Hello_1"}, {^^double, "Test_2"}});
}

template <typename T, size_t N>
using struct_of_arrays = mkr::struct_of_arrays_impl<T, N>::impl;

struct point {
	float x = 1;
	float y = 3;
	float z = 123;
};

template <> struct std::formatter<point> : mkr::universal_formatter {};

// A user-provided class.
struct Car {
	std::string make;
	std::string model;
	int year;
	std::vector<float> tire_pressure;
};

// Short forms

Car get_from() {
	std::string json = R"( { "make": "Toyota",
                              "model": "Camry",
                            "year": 2018,
        "tire_pressure": [ 40.1, 39.9 ] } )";
	return simdjson::from(simdjson::pad(json));
}

std::string make_string(const Car &c) { return simdjson::to_json(c); }

// More standard forms

Car get_standard() {
	std::string json = R"( { "make": "Toyota",
                              "model": "Camry",
                            "year": 2018,
        "tire_pressure": [ 40.1, 39.9 ] } )";
	simdjson::ondemand::parser parser;
	simdjson::ondemand::document doc = parser.iterate(simdjson::pad(json));
	Car c = doc.get<Car>();
	return c;
}

void simdjsontest() {
	std::string json = R"( { "make": "Toyota",
                              "model": "Camry",
                            "year": 2018,
        "tire_pressure": [ 40.1, 39.9 ] } )";
	Car c = simdjson::from(simdjson::pad(json));
	std::cout << simdjson::to_json(c) << "\n";
}

int main(int argc, char *argv[]) {
	auto A = R{.Hello_1 = 1, .Test_2 = 2.1};
	auto &[T1, T2] = A;
	std::println("{}:{}", T1, T2);

	using points = struct_of_arrays<point, 2>;

	points p = {.x = {1.1, 2.2}, .y = {3.3, 4.4}, .z = {5.5, 6.6}};

	for (size_t i = 0; i != 2; ++i) {
		std::println("({}, {}, {})", p.x[i], p.y[i], p.z[i]);
	}

	std::string OwO = {
#embed "test.txt"
	};
	std::println("{}", OwO);

	std::println("{}", point());
	simdjsontest();

	test();

	return 0;
}
