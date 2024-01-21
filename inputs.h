#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_INPUTS__
#define __MORE_INPUTS__

#include "config.h"

#include "prints.h"

#define _MORE_CRT_NAME inputs

_MORE_CRT_BEGIN
_MORE_DETAILS_BEGIN

_USING_MORE_NAMESPACE(prints);

template<class InStream, class... Args>
constexpr void finput(InStream&& is, Args&&... args) {
	((std::forward<InStream>(is) >> std::forward<Args>(args)), ...);
}

template<class... Args>
constexpr void input(Args&&... args) {
	finput(std::wcin, std::forward<Args>(args)...);
}

template<class InStream, class OutStream>
struct inputer {
	InStream& is;
	OutStream& os;

	constexpr inputer(InStream& is, OutStream& os) noexcept : is(is), os(os) {}

	template<class... Args>
	constexpr void operator()(Args&&... args) {
		finput(is, std::forward<Args>(args)...);
		fprint(os, std::forward<Args>(args)...);
	}
	template<class Arg>
	constexpr auto& operator>>(Arg&& arg) {
		is >> std::forward<Arg>(arg);
		fprint(os, std::forward<Arg>(arg));
		return is;
	}
	constexpr operator auto& () const noexcept {
		return is;
	}
	constexpr operator auto& () noexcept {
		return is;
	}
};

template<class InStream>
struct inputer<InStream, nullostream> {
	InStream& is;

	constexpr inputer(InStream& is, nullostream & = nullout) noexcept : is(is) {}

	template<class... Args>
	constexpr void operator()(Args&&... args) {
		finput(is, std::forward<Args>(args)...);
	}
	template<class Arg>
	constexpr auto& operator>>(Arg&& arg) {
		is >> std::forward<Arg>(arg);
		return is;
	}
	constexpr operator auto& () const noexcept {
		return is;
	}
	constexpr operator auto& () noexcept {
		return is;
	}
};

template<class InStream, class OutStream>
inputer(InStream&, OutStream&) -> inputer<InStream, OutStream>;
template<class InStream>
inputer(InStream&) -> inputer<InStream, nullostream>;

_MORE_DETAILS_END

using _MORE_DETAILS::finput;
using _MORE_DETAILS::input;
using _MORE_DETAILS::inputer;

_MORE_CRT_END

#undef _MORE_CRT_NAME
#endif // !__MORE_INPUTS__