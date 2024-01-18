#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_PRINTS__
#define __MORE_PRINTS__

#include "config.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>
#include <optional>
#include <variant>
#include "type_traits.h"

#if __has_include(<boost/pfr.hpp>)
#define __MORE_PRINTS_BOOST_PFR__
#include <boost/pfr.hpp>
#endif

namespace more::prints {
	namespace prints_details {
		using namespace more::type_traits;

		template<class T>
		constexpr bool is_key_value_v = requires() { T::key_type; T::value_type; };

		template<class T>
		constexpr bool is_rolled_v = requires(T && v) { std::begin(std::forward<T>(v)); std::end(std::forward<T>(v)); };

		template<class OutStream, class T>
		constexpr bool is_default_printable_v = requires(OutStream && os, T && v) { std::forward<OutStream>(os) << std::forward<T>(v); };

		template<class SepType, class EndType>
		struct printer_setting {
			SepType sep;
			EndType end;
		};
		template<>
		struct printer_setting<const char[2], const char[2]> {
			const char sep[2] = { ' ', '\0' };
			const char end[2] = { '\n', '\0' };
		};
		template<class SepType>
		struct printer_setting<SepType, const char[2]> {
			SepType sep;
			const char end[2] = { '\n', '\0' };
		};

		printer_setting()->printer_setting<const char[2], const char[2]>;
		template<class SepType>
		printer_setting(SepType) -> printer_setting<SepType, const char[2]>;
		template<class SepType, class EndType>
		printer_setting(SepType, EndType) -> printer_setting<SepType, EndType>;

		template<class T>
		concept kind_of_printer_setting = kind_of<T, printer_setting>;

		constexpr printer_setting printer_setting_default;
		constexpr printer_setting printer_setting_noline(" ", " ");

		template<class OutStream, class T>
		struct printer_adapter;

		template<class OutStream, class T>
		constexpr void print_once(OutStream&& os, T&& arg) {
			printer_adapter<OutStream, T>::print(std::forward<OutStream>(os), std::forward<T>(arg));
		}

		template<class OutStream, class T>
		struct printer_adapter {
		private:
			using T0 = std::remove_cvref_t<T>;

			template<size_t... Idx>
			constexpr static void _tuple_print(OutStream&& os, T&& tuple, std::index_sequence<Idx...>) {
				print_once(std::forward<OutStream>(os), "{");
				bool once = false;
				((once ? (print_once(std::forward<OutStream>(os), ", "), 0) : (once = true, 0), print_once(std::forward<OutStream>(os), std::get<Idx>(std::forward<T>(tuple)))), ...);
				print_once(std::forward<OutStream>(os), "}");
			}

			constexpr static void varient_print(OutStream&& os, T&& varient) {
				std::visit([&]<class Arg>(Arg&& arg) { print_once(std::forward<OutStream>(os), std::forward<Arg>(arg)); }, std::forward<T>(varient));
			}
			constexpr static void optional_print(OutStream&& os, T&& optional) {
				if (optional.has_value()) print_once(std::forward<OutStream>(os), *std::forward<T>(optional));
				else print_once(std::forward<OutStream>(os), std::nullopt);
			}
			constexpr static void char_print(OutStream&& os, T&& ch) {
				print_once(std::forward<OutStream>(os), { (T)'\'', std::forward<T>(ch), (T)'\'' , (T)'\0' });
			}
			constexpr static void str_print(OutStream&& os, T&& str) {
				print_once(std::forward<OutStream>(os), std::quoted(std::forward<T>(str)));
			}
			constexpr static void cstr_print(OutStream&& os, T&& cstr) {
				default_print(std::forward<OutStream>(os), std::forward<T>(cstr));
			}
			constexpr static void keyvalue_print(OutStream&& os, T&& keyvalue) {
				auto&& [k, v] = std::forward<T>(keyvalue);
				print_once(std::forward<OutStream>(os), k);
				print_once(std::forward<OutStream>(os), ": ");
				print_once(std::forward<OutStream>(os), v);
			}
			constexpr static void rolled_print(OutStream&& os, T&& cntr) {
				print_once(std::forward<OutStream>(os), "{");
				bool once = false;
				for (auto&& i : std::forward<T>(cntr))
				{
					if (once) print_once(std::forward<OutStream>(os), ", ");
					else once = true;
					print_once(std::forward<OutStream>(os), i);
				};
				print_once(std::forward<OutStream>(os), "}");
			}
			constexpr static void tuple_print(OutStream&& os, T&& tuple) {
				_tuple_print(std::forward<OutStream>(os), std::forward<T>(tuple), std::make_index_sequence<std::tuple_size_v<T0>>());
			}
			constexpr static void default_print(OutStream&& os, T&& arg) {
				std::forward<OutStream>(os) << std::forward<T>(arg);
			}
#ifdef __MORE_PRINTS_BOOST_PFR__
			constexpr static void aggregate_print(OutStream&& os, T&& aggregate) {
				print_once(std::forward<OutStream>(os), "{");
				bool once = false;
				boost::pfr::for_each_field(std::forward<T>(aggregate), [&](auto&& i) {
					if (once) print_once(std::forward<OutStream>(os), ", ");
					else once = true;
					print_once(std::forward<OutStream>(os), i);
					});
				print_once(std::forward<OutStream>(os), "}");
			}
#endif

		public:
			constexpr static void print(OutStream&& os, T&& arg) {
				if constexpr (is_nullopt_v<T0>) print_once(std::forward<OutStream>(os), "nullopt");
				else if constexpr (is_nullptr_v<T0>) print_once(std::forward<OutStream>(os), "nullptr");
				else if constexpr (is_monostate_v<T0>) print_once(std::forward<OutStream>(os), "monostate");
				else if constexpr (is_variant_v<T0>) varient_print(std::forward<OutStream>(os), std::forward<T>(arg));
				else if constexpr (is_optional_v<T0>) optional_print(std::forward<OutStream>(os), std::forward<T>(arg));
				else if constexpr (is_bool_v<T0>) print_once(std::forward<OutStream>(os), std::forward<T>(arg) ? "true" : "false");
				else if constexpr (is_char_v<T0>) char_print(std::forward<OutStream>(os), std::forward<T>(arg));
				else if constexpr (is_string_v<T0> || is_string_view_v<T0>) str_print(std::forward<OutStream>(os), std::forward<T>(arg));
				else if constexpr (is_chars_v<T0>) cstr_print(std::forward<OutStream>(os), std::forward<T>(arg));
				else if constexpr (is_key_value_v<T0>) keyvalue_print(std::forward<OutStream>(os), std::forward<T>(arg));
				else if constexpr (is_rolled_v<T>) rolled_print(std::forward<OutStream>(os), std::forward<T>(arg));
				else if constexpr (is_tuple_v<T0>) tuple_print(std::forward<OutStream>(os), std::forward<T>(arg));
				else if constexpr (is_default_printable_v<OutStream, T>) default_print(std::forward<OutStream>(os), std::forward<T>(arg));
#ifdef __MORE_PRINTS_BOOST_PFR__
				else if constexpr (is_aggregate_v<T0>) aggregate_print(std::forward<OutStream>(os), std::forward<T>(arg));
#endif
			}
		};

		template<kind_of_printer_setting Setting, class OutStream, class Arg, class... Args>
		constexpr void fprint(const Setting& setting, OutStream&& os, Arg&& arg, Args&&... args) {
			print_once(std::forward<OutStream>(os), std::forward<Arg>(arg));
			((print_once(std::forward<OutStream>(os), setting.sep), print_once(std::forward<OutStream>(os), std::forward<Args>(args))), ...);
			print_once(std::forward<OutStream>(os), setting.end);
		}
		template<kind_of_printer_setting Setting, class OutStream>
		constexpr void fprint(const Setting& setting, OutStream&& os) {
			print_once(std::forward<OutStream>(os), setting.end);
		}
		template<class OutStream, class... Args>
		constexpr void fprint(OutStream&& os, Args&&... args) {
			fprint(printer_setting_default, std::forward<OutStream>(os), std::forward<Args>(args)...);
		}
		template<kind_of_printer_setting Setting, class... Args>
		constexpr void print(const Setting& setting, Args&&... args) {
			fprint(setting, std::wcout, std::forward<Args>(args)...);
		}
		template<class... Args>
		constexpr void print(Args&&... args) {
			fprint(std::wcout, std::forward<Args>(args)...);
		}

		template<class OutStream>
		struct printer {
		private:
			OutStream& os;

		public:
			constexpr printer(OutStream& os) noexcept : os(os) {}

			template<kind_of_printer_setting Setting, class... Args>
			constexpr void operator()(const Setting& setting, Args&&... args) {
				fprint(setting, os, std::forward<Args>(args)...);
			}
			template<class... Args>
			constexpr void operator()(Args&&... args) {
				fprint(printer_setting_default, os, std::forward<Args>(args)...);
			}
			template<class Arg>
			constexpr decltype(auto) operator<<(Arg&& arg) {
				return os << std::forward<Arg>(arg);
			}
			constexpr operator const OutStream& () const noexcept {
				return os;
			}
			constexpr operator OutStream& () noexcept {
				return os;
			}
		};
	}

	using prints_details::is_default_printable_v;
	using prints_details::printer_setting;
	using prints_details::kind_of_printer_setting;
	using prints_details::printer_adapter;
	using prints_details::printer_setting_default;
	using prints_details::printer_setting_noline;
	using prints_details::fprint;
	using prints_details::print;
	using prints_details::printer;
}

#endif // !__MORE_PRINTS__
