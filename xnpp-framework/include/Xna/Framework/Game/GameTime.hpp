#ifndef XNA_FRAMEWORK_GAME_GAMETIME_HPP
#define XNA_FRAMEWORK_GAME_GAMETIME_HPP

#include "Xna/CSharp/TimeSpan.hpp"

namespace Xna {
	class GameTime {
	public:
		//Creates a new instance of GameTime.
		GameTime() = default;
		//Creates a new instance of GameTime.
		GameTime(CSharp::TimeSpan const& totalRealTime,
			CSharp::TimeSpan const& elapsedRealTime,
			CSharp::TimeSpan const& totalGameTime,
			CSharp::TimeSpan const& elapsedGameTime) 
			: totalRealTime(totalRealTime), elapsedRealTime(elapsedRealTime), totalGameTime(totalGameTime), elapsedGameTime(elapsedGameTime) {}
		//Creates a new instance of GameTime.
		GameTime(CSharp::TimeSpan const& totalRealTime,
			CSharp::TimeSpan const& elapsedRealTime,
			CSharp::TimeSpan const& totalGameTime,
			CSharp::TimeSpan const& elapsedGameTime,
			bool isRunningSlowly) 
			: totalRealTime(totalRealTime), elapsedRealTime(elapsedRealTime), totalGameTime(totalGameTime), elapsedGameTime(elapsedGameTime), isRunningSlowly(isRunningSlowly) {}

		//The amount of elapsed game time since the last update.
		CSharp::TimeSpan ElapsedGameTime() const { return elapsedGameTime; }
		//The amount of elapsed game time since the last update.
		void ElapsedGameTime(CSharp::TimeSpan const& value) { elapsedGameTime = value; }

		//The amount of elapsed real time (wall clock) since the last frame.
		CSharp::TimeSpan ElapsedRealTime() const { return elapsedRealTime; }
		//The amount of elapsed real time (wall clock) since the last frame.
		void ElapsedRealTime(CSharp::TimeSpan const& value) { elapsedRealTime = value; }

		//Gets a value indicating that the game loop is taking longer than its TargetElapsedTime. 
		//In this case, the game loop can be considered to be running too slowly and should do something to "catch up."
		bool IsRunningSlowly() const { return isRunningSlowly; }
		//Gets a value indicating that the game loop is taking longer than its TargetElapsedTime. 
		//In this case, the game loop can be considered to be running too slowly and should do something to "catch up."
		void IsRunningSlowly(bool value) { isRunningSlowly = value; }

		//The amount of game time since the start of the game.
		CSharp::TimeSpan TotalGameTime() const { return totalGameTime; }
		//The amount of game time since the start of the game.
		void TotalGameTime(CSharp::TimeSpan const& value) { totalGameTime = value; }

		//The amount of real time (wall clock) since the start of the game.
		CSharp::TimeSpan TotalRealTime() const { return totalRealTime; }
		//The amount of real time (wall clock) since the start of the game.
		void TotalRealTime(CSharp::TimeSpan const& value) { totalRealTime = value; }

	private:
		CSharp::TimeSpan totalRealTime{};
		CSharp::TimeSpan elapsedRealTime{};
		CSharp::TimeSpan totalGameTime{};
		CSharp::TimeSpan elapsedGameTime{};
		bool isRunningSlowly{ false };
	};
}

#endif