#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_TYPE_TRAITS__
#define __MORE_TYPE_TRAITS__

#include "config.h"

#include <string>
#include <optional>
#include <variant>

#define _MORE_CRT_NAME type_traits

_MORE_CRT_BEGIN
_MORE_DETAILS_BEGIN

template<class...>
constexpr bool always_false = false;
template<auto...>
constexpr bool always_false_v = false;

using std::disjunction_v;
using std::is_aggregate_v;
using std::is_array_v;
using std::is_pointer_v;
using std::is_same_v;
using std::is_void_v;

using std::decay_t;
using std::enable_if_t;
using std::remove_all_extents_t;
using std::remove_cvref_t;
using std::remove_pointer_t;

template<class T, class... Ts>
constexpr bool is_one_of_v = (is_same_v<T, Ts> || ...);
template<class T, class T0>
constexpr bool is_array_of_v = is_same_v<remove_all_extents_t<T>, T0>&& is_array_v<T>;
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
template<class T, bool B>
concept type_enable_if = B;

_MORE_DETAILS_END

using _MORE_DETAILS::always_false;
using _MORE_DETAILS::always_false_v;
using _MORE_DETAILS::disjunction_v;
using _MORE_DETAILS::is_aggregate_v;
using _MORE_DETAILS::is_array_of_v;
using _MORE_DETAILS::is_array_v;
using _MORE_DETAILS::is_bool_v;
using _MORE_DETAILS::is_chars_v;
using _MORE_DETAILS::is_char_array_v;
using _MORE_DETAILS::is_char_pointer_v;
using _MORE_DETAILS::is_char_v;
using _MORE_DETAILS::is_kind_of_v;
using _MORE_DETAILS::is_monostate_v;
using _MORE_DETAILS::is_nullopt_v;
using _MORE_DETAILS::is_nullptr_v;
using _MORE_DETAILS::is_one_of_v;
using _MORE_DETAILS::is_optional_v;
using _MORE_DETAILS::is_pointer_v;
using _MORE_DETAILS::is_same_v;
using _MORE_DETAILS::is_string_v;
using _MORE_DETAILS::is_string_view_v;
using _MORE_DETAILS::is_tuple_v;
using _MORE_DETAILS::is_variant_v;
using _MORE_DETAILS::is_void_v;

using _MORE_DETAILS::decay_t;
using _MORE_DETAILS::enable_if_t;
using _MORE_DETAILS::remove_all_extents_t;
using _MORE_DETAILS::remove_cvref_t;
using _MORE_DETAILS::remove_pointer_t;

using _MORE_DETAILS::kind_of;
using _MORE_DETAILS::type_enable_if;

_MORE_CRT_END

#undef _MORE_CRT_NAME
#endif // !__MORE_TYPE_TRAITS__