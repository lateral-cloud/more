#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_INPUTS__
#define __MORE_INPUTS__

#include "config.h"

#include "prints.h"

namespace more::inputs {
	namespace inputs_details {
		using namespace prints;

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

			constexpr inputer(InStream& is, nullostream& = nullout) noexcept : is(is) {}

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
	}

	using inputs_details::finput;
	using inputs_details::input;
	using inputs_details::inputer;
}

#endif // !__MORE_INPUTS__