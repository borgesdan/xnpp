#include "Xna/Framework/Game/GameClock.hpp"
#include "Xna/Platform/Platform.hpp"
#include <exception>

namespace Xna {
	void GameClock::Reset() {
		currentTimeBase = CSharp::TimeSpan::Zero();
		currentTimeOffset = CSharp::TimeSpan::Zero();
		baseRealTime = GameClock::Counter();
		lastRealTimeValid = false;
	}

	void GameClock::Step() {
		auto counter = GameClock::Counter();
		if (!lastRealTimeValid)
		{
			lastRealTime = counter;
			lastRealTimeValid = true;
		}

		try
		{
			currentTimeOffset = GameClock::CounterToTimeSpan(counter - baseRealTime);
		}
		catch (std::exception& ex)
		{
			currentTimeBase = currentTimeBase.Add(currentTimeOffset);
			baseRealTime = lastRealTime;
			try
			{
				currentTimeOffset = GameClock::CounterToTimeSpan(counter - baseRealTime);
			}
			catch (std::exception& ex)
			{
				baseRealTime = counter;
				currentTimeOffset = CSharp::TimeSpan::Zero();
			}
		}

		try
		{
			elapsedTime = GameClock::CounterToTimeSpan(counter - lastRealTime);
		}
		catch (std::exception& ex)
		{
			elapsedTime = CSharp::TimeSpan::Zero();
		}
		try
		{
			const auto num = lastRealTime + timeLostToSuspension;
			elapsedAdjustedTime = GameClock::CounterToTimeSpan(counter - num);
			timeLostToSuspension = 0L;
		}
		catch (std::exception& ex)
		{
			elapsedAdjustedTime = CSharp::TimeSpan::Zero();
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
		suspendStartTime = 0L;
	}

	int64_t GameClock::Counter() {
		return static_cast<int64_t>(Platform::System_GetClockCounter());
	}
	
	int64_t GameClock::Frequency() {
		return static_cast<int64_t>(Platform::System_GetClockFrequency());
	}

	CSharp::TimeSpan GameClock::CounterToTimeSpan(int64_t delta) {
		return CSharp::TimeSpan::FromTicks((delta * 10000000) / GameClock::Frequency());
	}
}