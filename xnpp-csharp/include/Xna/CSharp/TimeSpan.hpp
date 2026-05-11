#ifndef CSHARP_TIMESPAN_HPP
#define CSHARP_TIMESPAN_HPP

#include <cstdint>
#include <limits>
#include <cmath>
#include "Exception.hpp"
#include "SR.hpp"

namespace Xna::CSharp {
	struct TimeSpan {
		constexpr TimeSpan() = default;
		constexpr TimeSpan(int64_t ticks) : _ticks(ticks) {}

		constexpr TimeSpan(int32_t hours, int32_t minutes, int32_t seconds)
		{
			_ticks = TimeToTicks(hours, minutes, seconds);
		}

		constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds)
			: TimeSpan(days, hours, minutes, seconds, 0)
		{
		}

		constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds)
			: TimeSpan(days, hours, minutes, seconds, milliseconds, 0)
		{
		}

		constexpr TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds)
		{
			const auto totalMicroseconds = (days * MicrosecondsPerDay)
				+ (hours * MicrosecondsPerHour)
				+ (minutes * MicrosecondsPerMinute)
				+ (seconds * MicrosecondsPerSecond)
				+ (milliseconds * MicrosecondsPerMillisecond)
				+ microseconds;

			if ((totalMicroseconds > MaxMicroseconds) || (totalMicroseconds < MinMicroseconds))
			{
				throw ArgumentOutOfRangeException(SR::Overflow_TimeSpanTooLong);
			}
			_ticks = totalMicroseconds * TicksPerMicrosecond;
		}


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

		static constexpr TimeSpan Zero() { return TimeSpan(0); }
		static constexpr TimeSpan MaxValue() { return TimeSpan(MaxTicks); }
		static constexpr TimeSpan MinValue() { return TimeSpan(MinTicks); }

		constexpr int64_t Ticks() const { return _ticks; }
		constexpr int32_t Days() const { return static_cast<int32_t>(_ticks / TicksPerDay); }
		constexpr int32_t Hours() const { return static_cast<int32_t>(_ticks / TicksPerHour % HoursPerDay); }
		constexpr int32_t Milliseconds() const { return static_cast<int32_t>(_ticks / TicksPerMillisecond % MillisecondsPerSecond); }
		constexpr int32_t Microseconds() const { return static_cast<int32_t>(_ticks / TicksPerMicrosecond % MicrosecondsPerMillisecond); }
		constexpr int32_t Nanoseconds() const { return static_cast<int32_t>(_ticks % TicksPerMicrosecond * NanosecondsPerTick); }
		constexpr int32_t Minutes() const { return static_cast<int32_t>(_ticks / TicksPerMinute % MinutesPerHour); }
		constexpr int32_t Seconds() const { return static_cast<int32_t>(_ticks / TicksPerSecond % SecondsPerMinute); }
		constexpr double TotalDays() const { return static_cast<double>(_ticks) / TicksPerDay; }
		constexpr double TotalHours() const { return static_cast<double>(_ticks) / TicksPerHour; }

		constexpr double TotalMilliseconds() const
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

		constexpr double TotalMicroseconds() const { return static_cast<double>(_ticks) / TicksPerMicrosecond; }
		constexpr double TotalNanoseconds() const { return static_cast<double>(_ticks) * NanosecondsPerTick; }
		constexpr double TotalMinutes() const { return static_cast<double>(_ticks) / TicksPerMinute; }
		constexpr double TotalSeconds() const { return static_cast<double>(_ticks) / TicksPerSecond; }

		constexpr TimeSpan Add(TimeSpan const& ts) const { return *this + ts; }

		static constexpr TimeSpan FromDays(double value) {
			return Interval(value, TicksPerDay);
		}

		constexpr TimeSpan Duration() const {
			if (_ticks == MinTicks)
			{
				throw OverflowException(SR::Overflow_Duration);
			}

			return TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
		}

		constexpr static int64_t TimeToTicks(int32_t hour, int32_t minute, int32_t second) {
			const auto totalSeconds = (hour * SecondsPerHour)
				+ (minute * SecondsPerMinute)
				+ second;

			if ((totalSeconds > MaxSeconds) || (totalSeconds < MinSeconds))
			{
				throw ArgumentOutOfRangeException(SR::Overflow_TimeSpanTooLong);
			}
			return totalSeconds * TicksPerSecond;
		}

		static constexpr TimeSpan FromDays(int32_t days) {
			return FromUnits(days, TicksPerDay, MinDays, MaxDays);
		}

		static constexpr TimeSpan FromHours(int32_t hours) {
			return FromUnits(hours, TicksPerHour, MinHours, MaxHours);
		}

		static constexpr TimeSpan FromMinutes(int64_t minutes) {
			return FromUnits(minutes, TicksPerMinute, MinMinutes, MaxMinutes);
		}

		static constexpr TimeSpan FromSeconds(int64_t seconds) {
			return FromUnits(seconds, TicksPerSecond, MinSeconds, MaxSeconds);
		}

		static constexpr TimeSpan FromMicroseconds(int64_t microseconds) {
			return FromUnits(microseconds, TicksPerMicrosecond, MinMicroseconds, MaxMicroseconds);
		}

		static constexpr TimeSpan FromHours(double  value) {
			return Interval(value, TicksPerHour);
		}

		static constexpr TimeSpan FromMilliseconds(double value) {
			return Interval(value, TicksPerMillisecond);
		}

		static constexpr TimeSpan FromMicroseconds(double value) {
			return Interval(value, TicksPerMicrosecond);
		}

		static constexpr TimeSpan FromMinutes(double value) {
			return Interval(value, TicksPerMinute);
		}

		constexpr TimeSpan Negate() const {
			return -(*this);
		}

		static constexpr TimeSpan FromSeconds(double value) {
			return Interval(value, TicksPerSecond);
		}

		constexpr TimeSpan Subtract(TimeSpan const& ts) const {
			return *this - ts;
		}

		TimeSpan Multiply(double factor) const {
			return *this * factor;
		}

		TimeSpan Divide(double divisor) const {
			return *this / divisor;
		}

		double Divide(TimeSpan const& ts) const {
			return *this / ts;
		}

		static constexpr TimeSpan FromTicks(int64_t value) {
			return TimeSpan(value);
		}

		constexpr TimeSpan operator-() const {
			if (_ticks == MinTicks)
			{
				throw OverflowException(SR::Overflow_NegateTwosCompNum);
			}

			return TimeSpan(_ticks);
		}

		friend constexpr TimeSpan operator-(TimeSpan const& t1, TimeSpan const t2) {
			const auto result = t1._ticks - t2._ticks;
			const auto t1Sign = t1._ticks >> 63;

			if ((t1Sign != (t2._ticks >> 63)) && (t1Sign != (result >> 63)))
			{
				throw OverflowException(SR::Overflow_TimeSpanTooLong);
			}

			return TimeSpan(result);
		}

		constexpr TimeSpan operator+() const {
			return *this;
		}

		friend constexpr TimeSpan operator+(TimeSpan const& t1, TimeSpan const t2) {
			auto result = t1._ticks + t2._ticks;
			auto t1Sign = t1._ticks >> 63;

			if ((t1Sign == (t2._ticks >> 63)) && (t1Sign != (result >> 63)))
			{
				throw OverflowException(SR::Overflow_TimeSpanTooLong);
			}
			return TimeSpan(result);
		}

		friend TimeSpan operator*(TimeSpan const& timeSpan, double factor) {
			const auto ticks = std::round(timeSpan.Ticks() * factor);
			return IntervalFromDoubleTicks(ticks);
		}

		friend TimeSpan operator*(double factor, TimeSpan const& timeSpan) {
			return timeSpan * factor;
		}

		friend TimeSpan operator/(TimeSpan const& timeSpan, double divisor) {
			const auto ticks = std::round(timeSpan.Ticks() / divisor);
			return IntervalFromDoubleTicks(ticks);
		}

		friend double operator/(TimeSpan const& t1, TimeSpan const t2) {
			return t1.Ticks() / static_cast<double>(t2.Ticks());
		}

		constexpr bool operator==(TimeSpan const& other) const {
			return _ticks == other._ticks;
		}

		constexpr bool operator<(TimeSpan const& other) const {
			return _ticks < other._ticks;
		}

		constexpr bool operator<=(TimeSpan const& other) const {
			return _ticks <= other._ticks;
		}

		constexpr bool operator>(TimeSpan const& other) const {
			return _ticks > other._ticks;
		}

		constexpr bool operator>=(TimeSpan const& other) const {
			return _ticks >= other._ticks;
		}

	private:

		static constexpr TimeSpan FromUnits(int64_t units, int64_t ticksPerUnit, int64_t minUnits, int64_t maxUnits) {
			if (units > maxUnits || units < minUnits)
			{
				throw ArgumentOutOfRangeException(SR::Overflow_TimeSpanTooLong);
			}

			return TimeSpan::FromTicks(units * ticksPerUnit);
		}

		static constexpr TimeSpan Interval(double value, double scale) {
			return IntervalFromDoubleTicks(value * scale);
		}

		static constexpr TimeSpan IntervalFromDoubleTicks(double ticks) {
			if ((ticks > MaxTicks) || (ticks < MinTicks))
			{
				throw OverflowException(SR::Overflow_TimeSpanTooLong);
			}

			if (ticks == MaxTicks)
			{
				return MaxValue();
			}

			return TimeSpan(static_cast<int64_t>(ticks));
		}

	private:
		int64_t _ticks{ 0 };
	};

}

#endif