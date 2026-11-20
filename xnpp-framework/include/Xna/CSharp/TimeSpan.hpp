#pragma once

#include <cstdint>
#include <limits>
#include <cmath>
#include "Xna/Macros/CoreAssert.hpp"

namespace Xna {
	struct TimeSpan {
		int64_t _ticks{ 0 };

		constexpr TimeSpan() noexcept = default;
		constexpr TimeSpan(int64_t ticks) noexcept : _ticks(ticks) {}

		constexpr TimeSpan(int32_t hours, int32_t minutes, int32_t seconds) noexcept : _ticks(TimeToTicks(hours, minutes, seconds))
		{}

		constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds)
			noexcept : TimeSpan(days, hours, minutes, seconds, 0) {}

		constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds)
			noexcept : TimeSpan(days, hours, minutes, seconds, milliseconds, 0) {}

		constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds)
			noexcept : _ticks(TimeToTicks(days, hours, minutes, seconds, milliseconds, microseconds))
		{}

		static constexpr int64_t NanosecondsPerTick = 100;
		static constexpr int64_t TicksPerMicrosecond = 10;
		static constexpr int64_t TicksPerMillisecond = TicksPerMicrosecond * 1000;
		static constexpr int64_t TicksPerSecond = TicksPerMillisecond * 1000;
		static constexpr int64_t TicksPerMinute = TicksPerSecond * 60;
		static constexpr int64_t TicksPerHour = TicksPerMinute * 60;
		static constexpr int64_t TicksPerDay = TicksPerHour * 24;
		static constexpr int64_t MicrosecondsPerMillisecond = TicksPerMillisecond / TicksPerMicrosecond;
		static constexpr int64_t MicrosecondsPerSecond = TicksPerSecond / TicksPerMicrosecond;
		static constexpr int64_t MicrosecondsPerMinute = TicksPerMinute / TicksPerMicrosecond;
		static constexpr int64_t MicrosecondsPerHour = TicksPerHour / TicksPerMicrosecond;
		static constexpr int64_t MicrosecondsPerDay = TicksPerDay / TicksPerMicrosecond;
		static constexpr int64_t MillisecondsPerSecond = TicksPerSecond / TicksPerMillisecond;
		static constexpr int64_t MillisecondsPerMinute = TicksPerMinute / TicksPerMillisecond;
		static constexpr int64_t MillisecondsPerHour = TicksPerHour / TicksPerMillisecond;
		static constexpr int64_t MillisecondsPerDay = TicksPerDay / TicksPerMillisecond;
		static constexpr int64_t SecondsPerMinute = TicksPerMinute / TicksPerSecond;
		static constexpr int64_t SecondsPerHour = TicksPerHour / TicksPerSecond;
		static constexpr int64_t SecondsPerDay = TicksPerDay / TicksPerSecond;
		static constexpr int64_t MinutesPerHour = TicksPerHour / TicksPerMinute;
		static constexpr int64_t MinutesPerDay = TicksPerDay / TicksPerMinute;
		static constexpr int32_t HoursPerDay = static_cast<int32_t>(TicksPerDay / TicksPerHour);
		static constexpr int64_t MinTicks = (std::numeric_limits<int64_t>::min)();
		static constexpr int64_t MaxTicks = (std::numeric_limits<int64_t>::max)();
		static constexpr int64_t MinMicroseconds = MinTicks / TicksPerMicrosecond;
		static constexpr int64_t MaxMicroseconds = MaxTicks / TicksPerMicrosecond;
		static constexpr int64_t MinMilliseconds = MinTicks / TicksPerMillisecond;
		static constexpr int64_t MaxMilliseconds = MaxTicks / TicksPerMillisecond;
		static constexpr int64_t MinSeconds = MinTicks / TicksPerSecond;
		static constexpr int64_t MaxSeconds = MaxTicks / TicksPerSecond;
		static constexpr int64_t MinMinutes = MinTicks / TicksPerMinute;
		static constexpr int64_t MaxMinutes = MaxTicks / TicksPerMinute;
		static constexpr int64_t MinHours = MinTicks / TicksPerHour;
		static constexpr int64_t MaxHours = MaxTicks / TicksPerHour;
		static constexpr int64_t MinDays = MinTicks / TicksPerDay;
		static constexpr int64_t MaxDays = MaxTicks / TicksPerDay;
		static constexpr int64_t TicksPerTenthSecond = TicksPerMillisecond * 100;

		[[nodiscard]] static constexpr TimeSpan Zero() noexcept { return TimeSpan(0); }
		[[nodiscard]] static constexpr TimeSpan MaxValue() noexcept { return TimeSpan(MaxTicks); }
		[[nodiscard]] static constexpr TimeSpan MinValue() noexcept { return TimeSpan(MinTicks); }

		[[nodiscard]] constexpr int64_t Ticks() const noexcept { return _ticks; }
		[[nodiscard]] constexpr int32_t Days() const noexcept { return static_cast<int32_t>(_ticks / TicksPerDay); }
		[[nodiscard]] constexpr int32_t Hours() const noexcept { return static_cast<int32_t>(_ticks / TicksPerHour % HoursPerDay); }
		[[nodiscard]] constexpr int32_t Milliseconds() const noexcept { return static_cast<int32_t>(_ticks / TicksPerMillisecond % MillisecondsPerSecond); }
		[[nodiscard]] constexpr int32_t Microseconds() const noexcept { return static_cast<int32_t>(_ticks / TicksPerMicrosecond % MicrosecondsPerMillisecond); }
		[[nodiscard]] constexpr int32_t Nanoseconds() const noexcept { return static_cast<int32_t>(_ticks % TicksPerMicrosecond * NanosecondsPerTick); }
		[[nodiscard]] constexpr int32_t Minutes() const noexcept { return static_cast<int32_t>(_ticks / TicksPerMinute % MinutesPerHour); }
		[[nodiscard]] constexpr int32_t Seconds() const noexcept { return static_cast<int32_t>(_ticks / TicksPerSecond % SecondsPerMinute); }
		[[nodiscard]] constexpr double TotalDays() const noexcept { return static_cast<double>(_ticks) / TicksPerDay; }
		[[nodiscard]] constexpr double TotalHours() const noexcept { return static_cast<double>(_ticks) / TicksPerHour; }

		[[nodiscard]] constexpr double TotalMilliseconds() const noexcept
		{
			const double temp = static_cast<double>(_ticks) / TicksPerMillisecond;

			if (temp > MaxMilliseconds)
			{
				return MaxMilliseconds;
			}

			if (temp < MinMilliseconds)
			{
				return MinMilliseconds;
			}
			return temp;
		}

		[[nodiscard]] constexpr double TotalMicroseconds() const noexcept { return static_cast<double>(_ticks) / TicksPerMicrosecond; }
		[[nodiscard]] constexpr double TotalNanoseconds() const noexcept { return static_cast<double>(_ticks) * NanosecondsPerTick; }
		[[nodiscard]] constexpr double TotalMinutes() const noexcept { return static_cast<double>(_ticks) / TicksPerMinute; }
		[[nodiscard]] constexpr double TotalSeconds() const noexcept { return static_cast<double>(_ticks) / TicksPerSecond; }

		[[nodiscard]] constexpr TimeSpan Add(TimeSpan const& ts) const noexcept { return *this + ts; }

		[[nodiscard]] static constexpr TimeSpan FromDays(double value) noexcept {
			return Interval(value, TicksPerDay);
		}

		[[nodiscard]] constexpr TimeSpan Duration() const noexcept {
			XNPP_CORE_ASSERT(_ticks != MinTicks, "The duration cannot be returned for TimeSpan.MinValue because the absolute value of TimeSpan.MinValue exceeds the value of TimeSpan.MaxValue.");

			return TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
		}

		[[nodiscard]] constexpr static int64_t TimeToTicks(int32_t hour, int32_t minute, int32_t second) noexcept {
			const auto totalSeconds = (hour * SecondsPerHour)
				+ (minute * SecondsPerMinute)
				+ second;

			XNPP_CORE_ASSERT(!((totalSeconds > MaxSeconds) || (totalSeconds < MinSeconds)), "TimeSpan overflowed because the duration is too long.");

			return totalSeconds * TicksPerSecond;
		}

		[[nodiscard]] static constexpr int64_t TimeToTicks(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds) noexcept {
			const auto totalMicroseconds = (days * MicrosecondsPerDay)
				+ (hours * MicrosecondsPerHour)
				+ (minutes * MicrosecondsPerMinute)
				+ (seconds * MicrosecondsPerSecond)
				+ (milliseconds * MicrosecondsPerMillisecond)
				+ microseconds;

			XNPP_CORE_ASSERT(((totalMicroseconds > MaxMicroseconds) || (totalMicroseconds < MinMicroseconds)), "TimeSpan overflowed because the duration is too long.");

			return totalMicroseconds * TicksPerMicrosecond;
		}

		[[nodiscard]] static constexpr TimeSpan FromDays(int32_t days) noexcept {
			return FromUnits(days, TicksPerDay, MinDays, MaxDays);
		}

		[[nodiscard]] static constexpr TimeSpan FromHours(int32_t hours) noexcept {
			return FromUnits(hours, TicksPerHour, MinHours, MaxHours);
		}

		[[nodiscard]] static constexpr TimeSpan FromMinutes(int64_t minutes) noexcept {
			return FromUnits(minutes, TicksPerMinute, MinMinutes, MaxMinutes);
		}

		[[nodiscard]] static constexpr TimeSpan FromSeconds(int64_t seconds) noexcept {
			return FromUnits(seconds, TicksPerSecond, MinSeconds, MaxSeconds);
		}

		[[nodiscard]] static constexpr TimeSpan FromMicroseconds(int64_t microseconds) noexcept {
			return FromUnits(microseconds, TicksPerMicrosecond, MinMicroseconds, MaxMicroseconds);
		}

		[[nodiscard]] static constexpr TimeSpan FromHours(double  value) noexcept {
			return Interval(value, TicksPerHour);
		}

		[[nodiscard]] static constexpr TimeSpan FromMilliseconds(double value) noexcept {
			return Interval(value, TicksPerMillisecond);
		}

		[[nodiscard]] static constexpr TimeSpan FromMicroseconds(double value) noexcept {
			return Interval(value, TicksPerMicrosecond);
		}

		[[nodiscard]] static constexpr TimeSpan FromMinutes(double value) noexcept {
			return Interval(value, TicksPerMinute);
		}

		[[nodiscard]] constexpr TimeSpan Negate() const noexcept {
			return -(*this);
		}

		[[nodiscard]] static constexpr TimeSpan FromSeconds(double value) noexcept {
			return Interval(value, TicksPerSecond);
		}

		[[nodiscard]] constexpr TimeSpan Subtract(TimeSpan const& ts) const noexcept {
			return *this - ts;
		}

		[[nodiscard]] TimeSpan Multiply(double factor) const noexcept {
			return *this * factor;
		}

		[[nodiscard]] TimeSpan Divide(double divisor) const noexcept {
			return *this / divisor;
		}

		[[nodiscard]] double Divide(TimeSpan const& ts) const noexcept {
			return *this / ts;
		}

		[[nodiscard]] static constexpr TimeSpan FromTicks(int64_t value) noexcept {
			return TimeSpan(value);
		}

		[[nodiscard]] constexpr TimeSpan operator-() const noexcept {
			XNPP_CORE_ASSERT(_ticks != MinTicks, "The negation of TimeSpan.MinValue cannot be represented as a TimeSpan because the absolute value of TimeSpan.MinValue exceeds the value of TimeSpan.MaxValue.");

			return TimeSpan(_ticks);
		}

		[[nodiscard]] friend constexpr TimeSpan operator-(TimeSpan const& t1, TimeSpan const t2) noexcept {
			const auto result = t1._ticks - t2._ticks;
			const auto t1Sign = t1._ticks >> 63;

			XNPP_CORE_ASSERT(!((t1Sign != (t2._ticks >> 63)) && (t1Sign != (result >> 63))), "TimeSpan overflowed because the duration is too long.");

			return TimeSpan(result);
		}

		[[nodiscard]] constexpr TimeSpan operator+() const noexcept {
			return *this;
		}

		[[nodiscard]] friend constexpr TimeSpan operator+(TimeSpan const& t1, TimeSpan const t2) noexcept {
			auto result = t1._ticks + t2._ticks;
			auto t1Sign = t1._ticks >> 63;

			XNPP_CORE_ASSERT(!((t1Sign == (t2._ticks >> 63)) && (t1Sign != (result >> 63))), "TimeSpan overflowed because the duration is too long.");

			return TimeSpan(result);
		}

		[[nodiscard]] friend TimeSpan operator*(TimeSpan const& timeSpan, double factor) noexcept {
			const auto ticks = std::round(timeSpan.Ticks() * factor);
			return IntervalFromDoubleTicks(ticks);
		}

		[[nodiscard]] friend TimeSpan operator*(double factor, TimeSpan const& timeSpan) noexcept {
			return timeSpan * factor;
		}

		[[nodiscard]] friend TimeSpan operator/(TimeSpan const& timeSpan, double divisor) noexcept {
			const auto ticks = std::round(timeSpan.Ticks() / divisor);
			return IntervalFromDoubleTicks(ticks);
		}

		[[nodiscard]] friend double operator/(TimeSpan const& t1, TimeSpan const t2) noexcept {
			return t1.Ticks() / static_cast<double>(t2.Ticks());
		}

		[[nodiscard]] constexpr bool operator==(TimeSpan const& other) const noexcept {
			return _ticks == other._ticks;
		}

		[[nodiscard]] constexpr bool operator<(TimeSpan const& other) const noexcept {
			return _ticks < other._ticks;
		}

		[[nodiscard]] constexpr bool operator<=(TimeSpan const& other) const noexcept {
			return _ticks <= other._ticks;
		}

		[[nodiscard]] constexpr bool operator>(TimeSpan const& other) const noexcept {
			return _ticks > other._ticks;
		}

		[[nodiscard]] constexpr bool operator>=(TimeSpan const& other) const noexcept {
			return _ticks >= other._ticks;
		}

	private:

		[[nodiscard]] static constexpr TimeSpan FromUnits(int64_t units, int64_t ticksPerUnit, int64_t minUnits, int64_t maxUnits) noexcept {
			XNPP_CORE_ASSERT(!(units > maxUnits || units < minUnits), "TimeSpan overflowed because the duration is too long.");
			return TimeSpan::FromTicks(units * ticksPerUnit);
		}

		[[nodiscard]] static constexpr TimeSpan Interval(double value, double scale) noexcept {
			return IntervalFromDoubleTicks(value * scale);
		}

		[[nodiscard]] static constexpr TimeSpan IntervalFromDoubleTicks(double ticks) noexcept {
			XNPP_CORE_ASSERT(!((ticks > MaxTicks) || (ticks < MinTicks)), "TimeSpan overflowed because the duration is too long.");

			if (ticks == MaxTicks)
			{
				return MaxValue();
			}

			return TimeSpan(static_cast<int64_t>(ticks));
		}
	};
}