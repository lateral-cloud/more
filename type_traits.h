#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_TYPE_TRAITS__
#define __MORE_TYPE_TRAITS__

#include "config.h"

#include <string>
#include <optional>
#include <variant>

namespace more::type_traits {
	namespace type_traits_details {
		template<class>
		constexpr bool always_false = false;

		using std::disjunction_v;
		using std::is_aggregate_v;
		using std::is_array_v;
		using std::is_pointer_v;
		using std::is_same_v;
		using std::is_void_v;

		using std::decay_t;
		using std::remove_all_extents_t;
		using std::remove_cvref_t;
		using std::remove_pointer_t;

		template<class T, class... Ts>
		constexpr bool is_one_of_v = (is_same_v<T, Ts> || ...);
		template<class T, class T0>
		constexpr bool is_array_of_v = is_same_v<remove_all_extents_t<T>, T0> && is_array_v<T>;
		template<class, template<class...> class>
		constexpr bool is_kind_of_v = false;
		template<template<class...> class T, class... Params>
		constexpr bool is_kind_of_v<T<Params...>, T> = true;

		template<class T>
		constexpr bool is_bool_v = is_same_v<T, bool>;
		template<class T>
		constexpr bool is_char_v = is_one_of_v<T, char, wchar_t, signed char, unsigned char, char8_t, char16_t, char32_t>;
		template<class T>
		constexpr bool is_char_pointer_v = is_char_v<remove_cvref_t<remove_pointer_t<T>>> && is_pointer_v<T>;
		template<class T>
		constexpr bool is_char_array_v = is_char_v<remove_all_extents_t<T>> && is_array_v<T>;
		template<class T>
		constexpr bool is_chars_v = is_char_array_v<T> || is_char_pointer_v<T>;
		template<class T>
		constexpr bool is_monostate_v = is_same_v<T, std::monostate>;
		template<class T>
		constexpr bool is_nullopt_v = is_same_v<T, std::nullopt_t>;
		template<class T>
		constexpr bool is_nullptr_v = is_same_v<T, std::nullptr_t>;
		template<class T>
		constexpr bool is_optional_v = is_kind_of_v<T, std::optional>;
		template<class T>
		constexpr bool is_string_v = is_kind_of_v<T, std::basic_string>;
		template<class T>
		constexpr bool is_string_view_v = is_kind_of_v<T, std::basic_string_view>;
		template<class T>
		constexpr bool is_tuple_v = is_kind_of_v<T, std::tuple>;
		template<class T>
		constexpr bool is_variant_v = is_kind_of_v<T, std::variant>;

		template<class T, template<class...> class T0>
		concept kind_of = is_kind_of_v<T, T0>;
	}

	using type_traits_details::always_false;
	using type_traits_details::disjunction_v;
	using type_traits_details::is_aggregate_v;
	using type_traits_details::is_array_of_v;
	using type_traits_details::is_array_v;
	using type_traits_details::is_bool_v;
	using type_traits_details::is_chars_v;
	using type_traits_details::is_char_array_v;
	using type_traits_details::is_char_pointer_v;
	using type_traits_details::is_char_v;
	using type_traits_details::is_kind_of_v;
	using type_traits_details::is_monostate_v;
	using type_traits_details::is_nullopt_v;
	using type_traits_details::is_nullptr_v;
	using type_traits_details::is_one_of_v;
	using type_traits_details::is_optional_v;
	using type_traits_details::is_pointer_v;
	using type_traits_details::is_same_v;
	using type_traits_details::is_string_v;
	using type_traits_details::is_string_view_v;
	using type_traits_details::is_tuple_v;
	using type_traits_details::is_variant_v;
	using type_traits_details::is_void_v;

	using type_traits_details::decay_t;
	using type_traits_details::remove_all_extents_t;
	using type_traits_details::remove_cvref_t;
	using type_traits_details::remove_pointer_t;

	using type_traits_details::kind_of;
}

#endif // !__MORE_TYPE_TRAITS__