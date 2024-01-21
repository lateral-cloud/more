#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_PARAM_MANAGERS__
#define __MORE_PARAM_MANAGERS__

#include "config.h"

#include "type_traits.h"

#define _MORE_CRT_NAME param_managers

_MORE_CRT_BEGIN
_MORE_DETAILS_BEGIN

_USING_MORE_NAMESPACE(type_traits);

template<class... Params>
class param_manager {};

template<>
class param_manager<> {
	template<class T>
	struct combine_impl {};
	template<template<class...> class Wrap, class... Params>
	struct combine_impl<Wrap<Params...>> {
		using type = param_manager<Params...>;
	};

public:
	static constexpr size_t size = 0;

	template<class>
	static constexpr size_t count = 0;

	template<class T>
	using combine = combine_impl<T>::type;
	template<size_t Begin, class T>
	using insert_wrap = enable_if_t<Begin == 0, combine<T>>;
	template<size_t Begin, class... T>
	using insert = insert_wrap<Begin, param_manager<T...>>;
	template<size_t Begin, size_t N>
	using erase = enable_if_t<Begin == 0 && N == 0, param_manager>;
	template<size_t Begin, size_t N, class T>
	using emplace_wrap = enable_if_t<N == 0, insert_wrap<Begin, T>>;
	template<size_t Begin, size_t N, class... T>
	using emplace = emplace_wrap<Begin, N, param_manager<T...>>;
	template<class T>
	using push_back_wrap = insert_wrap<0, T>;
	template<class... T>
	using push_back = push_back_wrap<param_manager<T...>>;
	template<class T>
	using push_front_wrap = insert_wrap<0, T>;
	template<class... T>
	using push_front = push_front_wrap<param_manager<T...>>;

	template<template<class...> class Wrap>
	using wrapper = Wrap<>;
};

template<class T>
using to_param_manager = param_manager<>::combine<T>;

template<class Param, class... RestParams>
class param_manager<Param, RestParams...> {
public:
	using prev_type = param_manager<Param>;
	using rest_type = param_manager<RestParams...>;
	static constexpr size_t size = rest_type::size + 1;

private:
	template<size_t N>
	struct get_impl {
		using type = rest_type::template get<N - 1>;
	};
	template<>
	struct get_impl<0> {
		using type = Param;
	};

	template<class T>
	struct count_impl {
		static constexpr size_t value = is_same_v<Param, T> + rest_type::template count<T>;
	};

	template<class T>
	struct combine_impl {};
	template<template<class...> class Wrap, class... Params>
	struct combine_impl<Wrap<Params...>> {
		using type = param_manager<Param, RestParams..., Params...>;
	};
	template<size_t Begin, class T>
	struct insert_wrap_impl {
		using type = prev_type::template combine<rest_type::template insert_wrap<Begin - 1, T>>;
	};
	template<class T>
	struct insert_wrap_impl<0, T> {
		using type = param_manager<Param, RestParams...>;
	};
	template<template<class...> class T, class... Params>
	struct insert_wrap_impl<0, T<Params...>> {
		using type = param_manager<Params..., Param, RestParams...>;
	};
	template<size_t Begin, size_t N>
	struct erase_impl {
		using type = prev_type::template combine<rest_type::template erase<Begin - 1, N>>;
	};
	template<size_t N>
	struct erase_impl<0, N> {
		using type = rest_type::template erase<0, N - 1>;
	};
	template<>
	struct erase_impl<0, 0> {
		using type = param_manager;
	};

public:
	template<size_t N>
	using get = get_impl<N>::type;
	template<size_t... N>
	using gets = param_manager<get<N>...>;

	template<class... T>
	static constexpr size_t count = (count_impl<T>::value + ...);

	template<class T>
	using combine = combine_impl<T>::type;
	template<size_t Begin, class T>
	using insert_wrap = insert_wrap_impl<Begin, T>::type;
	template<size_t Begin, class... T>
	using insert = insert_wrap<Begin, param_manager<T...>>;
	template<size_t Begin, size_t N = 1>
	using erase = erase_impl<Begin, N>::type;
	template<size_t Begin, size_t N, class T>
	using emplace_wrap = insert_wrap<Begin, T>::template erase<Begin + to_param_manager<T>::size, N>;
	template<size_t Begin, size_t N, class... T>
	using emplace = emplace_wrap<Begin, N, param_manager<T...>>;
	template<class T>
	using push_back_wrap = insert_wrap<size - 1, T>;
	template<class... T>
	using push_back = push_back_wrap<param_manager<T...>>;
	template<class T>
	using push_front_wrap = insert_wrap<0, T>;
	template<class... T>
	using push_front = push_front_wrap<param_manager<T...>>;

	template<template<class...> class Wrap>
	using wrapper = Wrap<Param, RestParams...>;
};

_MORE_DETAILS_END

using _MORE_DETAILS::param_manager;

_MORE_CRT_END

#undef _MORE_CRT_NAME
#endif // !__MORE_PARAM_MANAGERS__