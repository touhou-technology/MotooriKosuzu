#ifndef REFLECTION_HPP
#define REFLECTION_HPP

#include <algorithm>
#include <array>
#include <format>
#include <meta>

namespace mkr {

template <typename E, bool Enumerable = std::meta::is_enumerable_type(^^E)>
	requires std::is_enum_v<E>
constexpr std::string_view enum_to_string(E value) {
	if constexpr (Enumerable)
		template for (constexpr auto e :
					  std::define_static_array(std::meta::enumerators_of(
						  ^^E))) if (value == [:e:]) return identifier_of(e);

	return "<unnamed>";
}

template <typename E, bool Enumerable = std::meta::is_enumerable_type(^^E)>
	requires std::is_enum_v<E>
constexpr std::optional<E> string_to_enum(std::string_view name) {
	if constexpr (Enumerable)
		template for (constexpr auto e :
					  std::define_static_array(std::meta::enumerators_of(
						  ^^E))) if (name ==
									 std::meta::identifier_of(e)) return [:e:];

	return std::nullopt;
}

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

// Test (Just for fun?)都是抄写的()
template <typename T, size_t N> struct struct_of_arrays_impl {
	struct impl;

	consteval {
		auto ctx = std::meta::access_context::current();

		std::vector<std::meta::info> old_members =
			nonstatic_data_members_of(^^T, ctx);
		std::vector<std::meta::info> new_members = {};
		for (auto member : old_members) {
			auto array_type = std::meta::substitute(
				^^std::array,
				{
					type_of(member), std::meta::reflect_constant(N)});
			auto mem_descr =
				data_member_spec(array_type, {.name = identifier_of(member)});
			new_members.push_back(mem_descr);
		}
		define_aggregate(^^impl, new_members);
	}
};

} // namespace mkr
#endif /* REFLECTION_HPP */
