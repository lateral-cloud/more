#ifdef _MSC_VER
#pragma once
#endif

#ifndef __MORE_PRINTS__
#define __MORE_PRINTS__

#include <concepts>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <string_view>
#include <optional>
#include <variant>
#include <boost/pfr.hpp>

namespace more::prints
{
	namespace prints_details
	{
		template<class SepType, class EndType> struct printer_setting
		{
			const SepType& sep;
			const EndType& end;
		};
		template<>							   struct printer_setting<const char[2], const char[2]>
		{
			const char(&sep)[2] = " ";
			const char(&end)[2] = "\n";
		};
		template<class SepType>				   struct printer_setting<SepType, const char[2]>
		{
			const SepType& sep;
			const char(&end)[2] = "\n";
		};

		template<class SepType, class EndType> printer_setting(SepType, EndType) -> printer_setting<SepType, EndType>;
											   printer_setting()				 -> printer_setting<const char[2], const char[2]>;
		template<class SepType>				   printer_setting(SepType)			 -> printer_setting<SepType, const char[2]>;

		constexpr printer_setting default_printer_setting;

		template<class T0, template<class...> class T> struct is_kind_of;
		template<class T>							   struct is_char_array;
		template<class OutStream, class T>			   struct printer;

		template<class T0, class T1>				   constexpr bool is_same_v = std::same_as<T0, T1>;
		template<class T0, class... Ts>				   constexpr bool is_one_of_v = (is_same_v<T0, Ts> || ...);
		template<class T0, template<class...> class T> constexpr bool is_kind_of_v = is_kind_of<T0, T>::value;
		template<class T>							   constexpr bool is_key_value_v = requires() { T::key_type; T::value_type; };
		template<class T>							   constexpr bool is_rolled_v = requires(const T& v) { std::begin(v); std::end(v); };
		template<class T>							   constexpr bool is_aggregate_v = std::is_aggregate_v<T>;
		template<class T>							   constexpr bool is_char_array_v = is_char_array<T>::value;

		template<class T> constexpr bool is_char_v = is_one_of_v<T,
			char, wchar_t, signed char, unsigned char, char8_t, char16_t, char32_t>;
		template<class T> constexpr bool is_char_pointer_v = is_one_of_v<T,
			char*, const char*, wchar_t*, const wchar_t*,
			signed char*, const signed char*, unsigned char*, const unsigned char*,
			char8_t*, const char8_t*, char16_t*, const char16_t*, char32_t*, const char32_t*>;
		template<class T> constexpr bool is_string_v = is_one_of_v<T,
			std::string, std::wstring, std::u8string, std::u16string, std::u32string,
			std::string_view, std::wstring_view, std::u8string_view, std::u16string_view, std::u32string_view>;
		template<class T> constexpr bool is_cstring_v = is_char_pointer_v<T> || is_char_array_v<T>;
		template<class T> concept char_type = is_char_v<T>;

		template<class T0, template<class...> class T>	  struct is_kind_of
		{
			static constexpr bool value = false;
		};
		template<template<class...> class T, class... Ts> struct is_kind_of<T<Ts...>, T>
		{
			static constexpr bool value = true;
		};
		template<class T>								  struct is_char_array
		{
			static constexpr bool value = false;
		};
		template<char_type T, size_t N>					  struct is_char_array<T[N]>
		{
			static constexpr bool value = true;
		};

		template<class OutStream, class T>
		void print_once(OutStream& os, const T& arg)
		{
			printer<OutStream, T>::print(os, arg);
		}

		template<class OutStream, class T> struct printer
		{
		private:
			using T0 = std::remove_cvref_t<T>;
			template<size_t... Idx>
			static void _tuple_print(OutStream& os, const T0& tuple, std::index_sequence<Idx...>)
			{
				print_once(os, "{");
				bool once = false;
				((once ? (print_once(os, ", "), 0) : (once = true, 0),
					printer<OutStream, decltype(std::tuple_element_t<Idx, T0>)>::print(os, std::get<Idx>(tuple))), ...);
				print_once(os, "}");
			}

			static void varient_print(OutStream& os, const T0& varient)
			{
				std::visit([&](const auto& v) { printer<OutStream, decltype(v)>::print(os, v); }, varient);
			}
			static void optional_print(OutStream& os, const T0& optional)
			{
				if (optional.has_value()) printer<OutStream, decltype(*optional)>::print(os, *optional);
				else printer<OutStream, std::nullopt_t>::print(os, std::nullopt);
			}
			static void char_print(OutStream& os, const T0& ch)
			{
				printer<OutStream, T0[]>::print(os, { (T0)'\'', ch, (T0)'\'' , (T0)'\0' });
			}
			static void str_print(OutStream& os, const T0& str)
			{
				printer<OutStream, decltype(std::quoted(str))>::print(os, std::quoted(str));
			}
			static void cstr_print(OutStream& os, const T0& cstr)
			{
				default_print(os, cstr);
			}
			static void keyvalue_print(OutStream& os, const T0& keyvalue)
			{
				auto& [k, v] = keyvalue;
				printer<OutStream, decltype(k)>::print(os, k);
				print_once(os, ": ");
				printer<OutStream, decltype(v)>::print(os, v);
			}
			static void rolled_print(OutStream& os, const T0& cntr)
			{
				print_once(os, "{");
				bool once = false;
				for (auto& i : cntr)
				{
					if (once) print_once(os, ", ");
					else once = true;
					printer<OutStream, decltype(i)>::print(os, i);
				};
				print_once(os, "}");
			}
			static void tuple_print(OutStream& os, const T0& tuple)
			{
				_tuple_print(os, tuple, std::make_index_sequence<std::tuple_size_v<T0>>());
			}
			static void aggregate_print(OutStream& os, const T0& aggregate)
			{
				print_once(os, "{");
				bool once = false;
				boost::pfr::for_each_field(aggregate, [&](auto& i) {
					if (once) print_once(os, ", ");
					else once = true;
					printer<OutStream, decltype(i)>::print(os, i);
					});
				print_once(os, "}");
			}
			static void default_print(OutStream& os, const T0& arg)
			{
				os << arg;
			}

		public:
			static void print(OutStream& os, const T0& arg)
			{
				if constexpr (is_same_v<T0, std::nullptr_t>) print_once(os, "nullptr");
				else if constexpr (is_same_v<T0, std::nullopt_t>) print_once(os, "nullopt");
				else if constexpr (is_same_v<T0, std::monostate>) print_once(os, "monostate");
				else if constexpr (is_kind_of_v<T0, std::variant>) varient_print(os, arg);
				else if constexpr (is_kind_of_v<T0, std::optional>) optional_print(os, arg);
				else if constexpr (is_same_v<T0, bool>) print_once(os, arg ? "true" : "false");
				else if constexpr (is_char_v<T0>) char_print(os, arg);
				else if constexpr (is_string_v<T0>) str_print(os, arg);
				else if constexpr (is_cstring_v<T0>) cstr_print(os, arg);
				else if constexpr (is_key_value_v<T0>) keyvalue_print(os, arg);
				else if constexpr (is_rolled_v<T0>) rolled_print(os, arg);
				else if constexpr (is_kind_of_v<T0, std::tuple>) tuple_print(os, arg);
				else if constexpr (is_aggregate_v<T0>) aggregate_print(os, arg);
				else default_print(os, arg);
			}
		};

		template<class SepType, class EndType, class OutStream, class Arg,  class... Args>
		void fprint(const printer_setting<SepType, EndType>& setting, OutStream& os, const Arg& arg, const Args&... args)
		{
			print_once(os, arg);
			((print_once(os, setting.sep), print_once(os, args)), ...);
			print_once(os, setting.end);
		}
		template<class OutStream, class... Args>
		void fprint(OutStream& os, const Args&... args)
		{
			fprint(default_printer_setting, os, args...);
		}
		template<class SepType, class EndType, class... Args>
		void print(const printer_setting<SepType, EndType>& setting, const Args&... args)
		{
			fprint(setting, std::wcout, args...);
		}
		template<class... Args>
		void print(const Args&... args)
		{
			fprint(default_printer_setting, std::wcout, args...);
		}
	}

	using prints_details::printer_setting;
	using prints_details::printer;
	using prints_details::fprint;
	using prints_details::print;
}

#endif // !__MORE_PRINTS__