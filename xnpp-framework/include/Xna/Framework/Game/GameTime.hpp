#pragma once

#include <Xna/CSharp/TimeSpan.hpp>

namespace Xna {
	struct GameTime {
		//Creates a new instance of GameTime.
		GameTime() noexcept = default;
		//Creates a new instance of GameTime.
		GameTime(TimeSpan const& totalRealTime,
			TimeSpan const& elapsedRealTime,
			TimeSpan const& totalGameTime,
			TimeSpan const& elapsedGameTime) 
			noexcept : TotalRealTime(totalRealTime), ElapsedRealTime(elapsedRealTime), TotalGameTime(totalGameTime), ElapsedGameTime(elapsedGameTime) {}
		//Creates a new instance of GameTime.
		GameTime(TimeSpan const& totalRealTime,
			TimeSpan const& elapsedRealTime,
			TimeSpan const& totalGameTime,
			TimeSpan const& elapsedGameTime,
			bool isRunningSlowly) 
			noexcept : TotalRealTime(totalRealTime), ElapsedRealTime(elapsedRealTime), TotalGameTime(totalGameTime), ElapsedGameTime(elapsedGameTime), IsRunningSlowly(isRunningSlowly) {}

		//The amount of elapsed game time since the last update.
		TimeSpan ElapsedGameTime{};
		//The amount of elapsed real time (wall clock) since the last frame.
		TimeSpan ElapsedRealTime{};
		//Gets a value indicating that the game loop is taking longer than its TargetElapsedTime. 
		//In this case, the game loop can be considered to be running too slowly and should do something to "catch up."
		bool IsRunningSlowly { false };
		//The amount of game time since the start of the game.
		TimeSpan TotalGameTime{};
		//The amount of real time (wall clock) since the start of the game.
		TimeSpan TotalRealTime{};
	};
}