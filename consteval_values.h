#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_CONSTEVAL_VALUES__
#define __MORE_CONSTEVAL_VALUES__

#include "config.h"

#define _MORE_CRT_NAME consteval_values

_MORE_CRT_BEGIN
_MORE_DETAILS_BEGIN

template<auto Value>
struct consteval_value {
	using value_type = decltype(Value);

	constexpr consteval_value() noexcept = default;

	static constexpr value_type value() noexcept {
		return Value;
	}
	constexpr operator value_type() const noexcept {
		return Value;
	}
	_NODISCARD constexpr value_type operator()() const noexcept {
		return Value;
	}

	template<auto Value2>
	_NODISCARD constexpr auto operator*() const noexcept {
		return consteval_value<*Value>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator->() const noexcept {
		return consteval_value<operator->(Value)()>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator~() const noexcept {
		return consteval_value<~Value>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator+() const noexcept {
		return consteval_value<+Value>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator-() const noexcept {
		return consteval_value<-Value>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator++() const noexcept {
		return consteval_value<++Value>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator--() const noexcept {
		return consteval_value<--Value>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator++(int) const noexcept {
		return consteval_value<Value++>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator--(int) const noexcept {
		return consteval_value<Value-->();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator+(consteval_value<Value2>) const noexcept {
		return consteval_value<Value + Value2>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator-(consteval_value<Value2>) const noexcept {
		return consteval_value<Value - Value2>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator*(consteval_value<Value2>) const noexcept {
		return consteval_value<Value * Value2>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator/(consteval_value<Value2>) const noexcept {
		return consteval_value<Value / Value2>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator|(consteval_value<Value2>) const noexcept {
		return consteval_value<Value | Value2>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator&(consteval_value<Value2>) const noexcept {
		return consteval_value<Value & Value2>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator^(consteval_value<Value2>) const noexcept {
		return consteval_value<Value ^ Value2>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator<(consteval_value<Value2>) const noexcept {
		return consteval_value<(Value < Value2)>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator>(consteval_value<Value2>) const noexcept {
		return consteval_value<(Value > Value2)>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator<=(consteval_value<Value2>) const noexcept {
		return consteval_value<(Value <= Value2)>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator>=(consteval_value<Value2>) const noexcept {
		return consteval_value<(Value >= Value2)>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator==(consteval_value<Value2>) const noexcept {
		return consteval_value<(Value == Value2)>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator!=(consteval_value<Value2>) const noexcept {
		return consteval_value<(Value != Value2)>();
	}
	template<auto Value2>
	_NODISCARD constexpr auto operator<=>(consteval_value<Value2>) const noexcept {
		return consteval_value<(Value <=> Value2)>();
	}
};

_MORE_DETAILS_END

using _MORE_DETAILS::consteval_value;
using consteval_true = consteval_value<true>;
using consteval_false = consteval_value<false>;

_MORE_CRT_END

#undef _MORE_CRT_NAME
#endif // !__MORE_CONSTEVAL_VALUES__