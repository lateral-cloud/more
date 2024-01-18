#ifdef _MSC_VER
#pragma once
#endif // !_MSC_VER

#ifndef __MORE_SCOPE_GUARDS__
#define __MORE_SCOPE_GUARDS__

#include "config.h"

#include "times.h"
#include "inputs.h"
#include "prints.h"

namespace more::scope_guards {
	namespace scope_guards_details {
		using namespace more::inputs;
		using namespace more::prints;

		template<class Clock, class InStream, class OutStream>
		struct scope_guard {
		private:
			InStream& is;
			OutStream& os;
			const std::wstring name;

			static auto make_time_str() {
				times::times_t<Clock> t;
				t.get_times();
				return std::format(L"{:04}/{:02}/{:02} {:02}:{:02}:{:02}.{:03}",
					t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
					t.tm_hour, t.tm_min, t.tm_sec, t.tm_milisec);
			}

		public:
			template<kind_of_printer_setting Setting, class... Args>
			void print_tip(std::wstring_view tip, const Setting& setting, Args&&... args) const {
				std::wstring tips = L"[" + make_time_str() + L" | " + name + L": " + tip.data() + L"]";
				fprint(setting, os, tips.data(), std::forward<Args>(args)...);
			}
			template<class... Args>
			void print_tip(std::wstring_view tip, Args&&... args) const {
				print_tip(tip, printer_setting_default, std::forward<Args>(args)...);
			}

			template<kind_of_printer_setting Setting, class... Args>
			void print(const Setting& setting, Args&&... args) const {
				print_tip(L"print", setting, std::forward<Args>(args)...);
			}
			template<class... Args>
			void print(Args&&... args) const {
				print(printer_setting_default, std::forward<Args>(args)...);
			}
			template<kind_of_printer_setting Setting, class... Args>
			void input(const Setting& setting, Args&&... args) const {
				print_tip(L"input", setting);
				finput(is, std::forward<Args>(args)...);
			}
			template<class... Args>
			void input(Args&&... args) const {
				input(printer_setting_noline, std::forward<Args>(args)...);
			}
			template<kind_of_printer_setting Setting, class... Args>
			void exit(const Setting& setting, int code = 0, Args&&... args) const {
				print_tip(L"exit", setting, std::forward<Args>(args)...);
				std::exit(code);
			}
			template<class... Args>
			void exit(int code = 0, Args&&... args) const {
				exit(code, printer_setting_default, std::forward<Args>(args)...);
			}
			template<kind_of_printer_setting Setting, class... Args>
			void quick_exit(const Setting& setting, int code = 0, Args&&... args) const {
				print_tip(L"quick_exit", setting, std::forward<Args>(args)...);
				std::quick_exit(code);
			}
			template<class... Args>
			void quick_exit(int code = 0, Args&&... args) const {
				quick_exit(code, printer_setting_default, std::forward<Args>(args)...);
			}
			template<kind_of_printer_setting Setting, class... Args>
			void abort(const Setting& setting, Args&&... args) const {
				print_tip(L"abort", std::forward<Args>(args)...);
				std::abort();
			}
			template<class... Args>
			void abort(Args&&... args) const {
				abort(printer_setting_default, std::forward<Args>(args)...);
			}
			template<kind_of_printer_setting Setting, class... Args>
			void terminate(const Setting& setting, Args&&... args) const {
				print_tip(L"terminate", setting, std::forward<Args>(args)...);
				std::terminate();
			}
			template<class... Args>
			void terminate(Args&&... args) const {
				terminate(printer_setting_default, std::forward<Args>(args)...);
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

#endif // !__MORE_SCOPE_GUARDS__