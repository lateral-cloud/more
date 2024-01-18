#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_BIT_CHOOSE__
#define __MORE_BIT_CHOOSE__

#include "config.h"

namespace more::bit_choose {
	namespace bit_choose_details {
		template<size_t value>
		struct bit_choice {
			constexpr operator size_t() const noexcept {
				return value;
			}

			template<size_t value2> constexpr auto operator|(bit_choice<value2>) const noexcept {
				return bit_choice<value | value2>();
			}
			template<size_t value2> constexpr auto operator&(bit_choice<value2>) const noexcept {
				return bit_choice<value & value2>();
			}
			template<size_t value2> constexpr auto operator^(bit_choice<value2>) const noexcept {
				return bit_choice<value ^ value2>();
			}
			template<size_t value2> constexpr auto operator+(bit_choice<value2> choice) const noexcept {
				return *this | choice;
			}
			template<size_t value2> constexpr auto operator-(bit_choice<value2> choice) const noexcept {
				return *this ^ choice & *this;
			}

			template<size_t value2> constexpr bool has_chosen(bit_choice<value2> choice) const noexcept {
				return *this & choice;
			}
			template<size_t value2> constexpr bool has_not_chosen(bit_choice<value2> choice) const noexcept {
				return !has_chosen(choice);
			}
		};

		template<size_t count>
		constexpr auto choose_bit() noexcept {
			return bit_choice<(1 << count)>();
		}
	}

	using bit_choose_details::bit_choice;
	using bit_choose_details::choose_bit;
}

#endif // !__MORE_BIT_CHOOSE__