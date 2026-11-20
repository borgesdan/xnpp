#ifndef XNA_FRAMEWORK_GAME_GAMECLOCK_HPP
#define XNA_FRAMEWORK_GAME_GAMECLOCK_HPP

#include <Xna/CSharp/TimeSpan.hpp>
#include <cstdint>
#include "Xna/Macros/Api.hpp"

namespace Xna {
	//Internal game clock
	struct GameClock {
	public:
		inline GameClock() { Reset(); }

		constexpr TimeSpan CurrentTime() const { return currentTimeBase + currentTimeOffset; }
		constexpr TimeSpan ElapsedTime() const { return elapsedTime; }
		constexpr TimeSpan ElapsedAdjustedTime() const { return elapsedAdjustedTime; }

		XNPP_API void Reset();
		XNPP_API void Step();
		XNPP_API void Suspend();
		XNPP_API void Resume();

		XNPP_API static int64_t Counter();
		XNPP_API static int64_t Frequency();

		XNPP_API static TimeSpan CounterToTimeSpan(int64_t delta);

	private:
		int64_t baseRealTime{ 0 };
		int64_t lastRealTime{ 0 };
		bool lastRealTimeValid{ false };
		int32_t suspendCount;
		int64_t suspendStartTime{ 0 };
		int64_t timeLostToSuspension{ 0 };
		TimeSpan currentTimeOffset;
		TimeSpan currentTimeBase;
		TimeSpan elapsedTime;
		TimeSpan elapsedAdjustedTime;
	};
}

#endif