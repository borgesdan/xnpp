#include <Xna/Framework/Game/Game.hpp>
#include <limits>

namespace Xna {
	namespace {
		constexpr int64_t TicksPerSecond = 10000000; // 1 tick = 100 nanossegundos (padrão XNA/.NET)
	}

	void GameClock::Reset() {
		currentTimeBase = TimeSpan::Zero();
		currentTimeOffset = TimeSpan::Zero();
		baseRealTime = GameClock::Counter();
		lastRealTimeValid = false;
	}

	void GameClock::Step() {
		const int64_t counter = GameClock::Counter();
		if (!lastRealTimeValid) {
			lastRealTime = counter;
			lastRealTimeValid = true;
		}

		if (!TryCounterToTimeSpan(counter - baseRealTime, currentTimeOffset)) {
			currentTimeBase = currentTimeBase.Add(currentTimeOffset);
			baseRealTime = lastRealTime;

			if (!TryCounterToTimeSpan(counter - baseRealTime, currentTimeOffset)) {
				baseRealTime = counter;
				currentTimeOffset = TimeSpan::Zero();
			}
		}

		if (!TryCounterToTimeSpan(counter - lastRealTime, elapsedTime)) {
			elapsedTime = TimeSpan::Zero();
		}

		const int64_t adjustedLastTime = lastRealTime + timeLostToSuspension;
		if (TryCounterToTimeSpan(counter - adjustedLastTime, elapsedAdjustedTime)) {
			timeLostToSuspension = 0;
		}
		else {
			elapsedAdjustedTime = TimeSpan::Zero();
		}

		lastRealTime = counter;
	}

	void GameClock::Suspend() {
		++suspendCount;
		if (suspendCount != 1)
			return;
		suspendStartTime = GameClock::Counter();
	}

	void GameClock::Resume() {
		--suspendCount;
		if (suspendCount > 0)
			return;
		timeLostToSuspension += GameClock::Counter() - suspendStartTime;
		suspendStartTime = 0;
	}

	int64_t GameClock::Counter() {
		using Clock = std::chrono::steady_clock;
		return static_cast<int64_t>(Clock::now().time_since_epoch().count());
	}

	int64_t GameClock::Frequency() {
		using Period = std::chrono::steady_clock::period;
		return static_cast<int64_t>(Period::den / Period::num);
	}

	bool GameClock::TryCounterToTimeSpan(int64_t delta, TimeSpan& outTimeSpan) noexcept {
		const int64_t freq = GameClock::Frequency();
		if (freq <= 0) {
			outTimeSpan = TimeSpan::Zero();
			return false;
		}

		constexpr int64_t maxInt64 = std::numeric_limits<int64_t>::max();
		constexpr int64_t minInt64 = std::numeric_limits<int64_t>::min();

		// Separa segundos inteiros do resto para evitar overflow prematuro em (delta * TicksPerSecond)
		const int64_t wholeSeconds = delta / freq;
		const int64_t remainder = delta % freq;

		if (wholeSeconds > maxInt64 / TicksPerSecond || wholeSeconds < minInt64 / TicksPerSecond) {
			return false;
		}

		const int64_t wholeTicks = wholeSeconds * TicksPerSecond;

		int64_t remainderTicks = 0;
		if (remainder > maxInt64 / TicksPerSecond || remainder < minInt64 / TicksPerSecond) {
			// Fallback seguro caso a frequência do relógio seja extremamente alta
			remainderTicks = static_cast<int64_t>(
				(static_cast<long double>(remainder) * TicksPerSecond) / static_cast<long double>(freq)
				);
		}
		else {
			remainderTicks = (remainder * TicksPerSecond) / freq;
		}

		// Verifica se a soma final ultrapassa os limites de int64_t
		if ((remainderTicks > 0 && wholeTicks > maxInt64 - remainderTicks) ||
			(remainderTicks < 0 && wholeTicks < minInt64 - remainderTicks)) {
			return false;
		}

		outTimeSpan = TimeSpan::FromTicks(wholeTicks + remainderTicks);
		return true;
	}

	TimeSpan GameClock::CounterToTimeSpan(int64_t delta) {
		TimeSpan result{};
		if (TryCounterToTimeSpan(delta, result)) {
			return result;
		}
		return delta >= 0
			? TimeSpan::FromTicks(std::numeric_limits<int64_t>::max())
			: TimeSpan::FromTicks(std::numeric_limits<int64_t>::min());
	}
}