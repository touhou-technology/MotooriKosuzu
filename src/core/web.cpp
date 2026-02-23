#include "web.h"

#include <iostream>
#include <meta>
#include <vector>

std::vector<std::string> UseAPI::ReflectionAPI() {
	std::vector<std::string> result = {};

	template for (constexpr auto m : std::define_static_array(
					                                          enumerators_of(^^UseAPI::TranslateAPI))) {
	result.push_back(std::string(identifier_of(m)));
}

	return result;
}
