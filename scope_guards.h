#ifdef _MSC_VER
#pragma once
#endif

#ifndef __MORE_SCOPE_GUARDS__
#define __MORE_SCOPE_GUARDS__

#include "config.h"

#include "times.h"
#include "prints.h"

namespace more::scope_guards {
	namespace scope_guards_details {
		template<class Clock, class InStream, class OutStream>
		struct scope_guard {
		private:
			InStream& is;
			OutStream& os;
			const std::wstring name;

			static auto make_time_str() {
				times::times_t<Clock> t;
				t.get_times();
				return std::format(L"{:04}/{:02}/{:02} {:02}:{:02}:{:02}.{:03}{:03}",
					t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
					t.tm_hour, t.tm_min, t.tm_sec, t.tm_milisec, t.tm_microsec);
			}

		public:
			void print_tip(std::wstring_view tip, const auto&... args) const {
				std::wstring words = L"[" + name + L": " + tip.data() + L"]";
				if (sizeof...(args))
					words += L":";
				more::prints::fprint(os, make_time_str().data(), words.data(), args...);
			}

			void print(const auto&... args) const {
				print_tip(L"print", args...);
			}
			void input(auto&... args) const {
				print_tip(L"input");
				((is >> args), ...);
			}
			void exit(const auto&... args, int code = 0) const {
				print_tip(L"exit", args...);
				std::exit(code);
			}
			void quick_exit(const auto&... args, int code = 0) const {
				print_tip(L"quick_exit", args...);
				std::quick_exit(code);
			}
			void abort(const auto&... args) const {
				print_tip(L"abort", args...);
				std::abort();
			}
			void terminate(const auto&... args) const {
				print_tip(L"terminate", args...);
				std::terminate();
			}

			void sleep_for(int ms) const {
				std::this_thread::sleep_for(std::chrono::milliseconds(ms));
			}
			void sleep_for_char() const {
				(void)getchar();
				(void)getchar();
			}

			scope_guard(InStream& is, OutStream& os, std::wstring_view name) : is(is), os(os), name(name) {
				print_tip(L"begin");
			}
			~scope_guard() {
				print_tip(L"end");
			}
		};

		template<class InStream, class OutStream, class NameType>
		scope_guard(InStream&, OutStream&, NameType) -> scope_guard<std::chrono::system_clock, InStream, OutStream>;
	}

	using scope_guards_details::scope_guard;
}

#endif