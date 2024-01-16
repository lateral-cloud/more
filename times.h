#ifdef _MSC_VER
#pragma once
#endif

#ifndef __MORE_TIMES__
#define __MORE_TIMES__

#include "config.h"

#include <chrono>

namespace more::times
{
	namespace times_details
	{
		template<class Clock = std::chrono::system_clock>
		struct times_t
		{
			int tm_nanosec;  // nanoseconds after the microsecond - [0, 999]
			int tm_microsec; // microseconds after the milisecond - [0, 999]
			int tm_milisec;  // miliseconds after the second - [0, 999]
			int tm_sec;      // seconds after the minute - [0, 60]
			int tm_min;      // minutes after the hour - [0, 59]
			int tm_hour;     // hours since midnight - [0, 23]
			int tm_mday;     // day of the month - [1, 31]
			int tm_mon;      // months since January - [0, 11]
			int tm_year;     // years since 1900
			int tm_wday;     // days since Sunday - [0, 6]
			int tm_yday;     // days since January 1 - [0, 365]
			int tm_isdst;    // daylight savings time flag

			void get_times() noexcept
			{
				tm t;
				const auto duration = Clock::now().time_since_epoch();
				const auto time = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
				const auto mili = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
				const auto micro = std::chrono::duration_cast<std::chrono::microseconds>(duration).count() % 1000;
				const auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() % 1000;
				localtime_s(&t, &time);
				tm_nanosec = (int)nano;
				tm_microsec = (int)micro;
				tm_milisec = (int)mili;
				tm_sec = t.tm_sec;
				tm_min = t.tm_min;
				tm_hour = t.tm_hour;
				tm_mday = t.tm_mday;
				tm_mon = t.tm_mon;
				tm_year = t.tm_year;
				tm_wday = t.tm_wday;
				tm_yday = t.tm_yday;
				tm_isdst = t.tm_isdst;
			}
		};
	}

	using times_details::times_t;
}

#endif