#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_INPUTS__
#define __MORE_INPUTS__

#include "config.h"

#include <iostream>
#include "type_traits.h"

namespace more::inputs {
	namespace inputs_details {
		template<class InStream, class... Args>
		constexpr void finput(InStream&& is, Args&&... args) {
			((std::forward<InStream>(is) >> std::forward<Args>(args)), ...);
		}

		template<class... Args>
		constexpr void input(Args&&... args) {
			finput(std::wcin, std::forward<Args>(args)...);
		}

		template<class InStream>
		struct inputer {
		private:
			InStream& is;

		public:
			constexpr inputer(InStream& is) noexcept : is(is) {}

			template<class... Args>
			constexpr void operator()(Args&&... args) {
				finput(is, std::forward<Args>(args)...);
			}
			template<class Arg>
			constexpr decltype(auto) operator>>(Arg&& arg) {
				return is >> std::forward<Arg>(arg);
			}
			constexpr operator const InStream& () const noexcept {
				return is;
			}
			constexpr operator InStream& () noexcept {
				return is;
			}
		};
	}

	using inputs_details::finput;
	using inputs_details::input;
	using inputs_details::inputer;
}

#endif // !__MORE_INPUTS__