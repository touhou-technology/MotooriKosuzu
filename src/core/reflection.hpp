#ifndef REFLECTION_HPP
#define REFLECTION_HPP

#include <algorithm>
#include <meta>

consteval auto make_named_tuple(
	std::meta::info type,
	std::initializer_list<std::pair<std::meta::info, std::string_view>>
		members) {
	std::vector<std::meta::info> nsdms;
	for (auto [type, name] : members) {
		nsdms.push_back(std::meta::data_member_spec(type, {.name = name}));
	}
	return std::meta::define_aggregate(type, nsdms);
}

#endif /* REFLECTION_HPP */
