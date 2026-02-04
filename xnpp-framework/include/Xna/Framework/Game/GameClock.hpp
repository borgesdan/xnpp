#ifndef XNA_FRAMEWORK_GAME_GAMECLOCK_HPP
#define XNA_FRAMEWORK_GAME_GAMECLOCK_HPP

#include "Xna/CSharp/TimeSpan.hpp"
#include <cstdint>

namespace Xna {
	//Internal game clock
	struct GameClock {
	public:
		inline GameClock() { Reset(); }

		constexpr CSharp::TimeSpan CurrentTime() const { return currentTimeBase + currentTimeOffset; }
		constexpr CSharp::TimeSpan ElapsedTime() const { return elapsedTime; }
		constexpr CSharp::TimeSpan ElapsedAdjustedTime() const { return elapsedAdjustedTime; }

		void Reset();
		void Step();
		void Suspend();
		void Resume();

		static int64_t Counter();
		static int64_t Frequency();

		static CSharp::TimeSpan CounterToTimeSpan(int64_t delta);

	private:
		int64_t baseRealTime{ 0 };
		int64_t lastRealTime{ 0 };
		bool lastRealTimeValid{ false };
		int32_t suspendCount;
		int64_t suspendStartTime{ 0 };
		int64_t timeLostToSuspension{ 0 };
		CSharp::TimeSpan currentTimeOffset;
		CSharp::TimeSpan currentTimeBase;
		CSharp::TimeSpan elapsedTime;
		CSharp::TimeSpan elapsedAdjustedTime;
	};
}

#endif