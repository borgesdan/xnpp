#ifndef XNA_FRAMEWORK_GAME_GAME_HPP
#define XNA_FRAMEWORK_GAME_GAME_HPP

#include "_Game.hpp"

#include "GameComponentCollection.hpp"
#include "Xna/Framework/Content/ContentManager.hpp"
#include "Xna/Framework/Game/GraphicsDeviceManager.hpp"
#include "LaunchParameters.hpp"
#include "GameServiceContainer.hpp"
#include "GameWindow.hpp"
#include "GameTime.hpp"
#include "IGameComponent.hpp"
#include "DrawableGameComponent.hpp"

#include "GameClock.hpp"
#include "IGraphicsDeviceManager.hpp"
#include "Xna/Framework/Graphics/IGraphicsDeviceService.hpp"
#include "GameHost.hpp"
#include <vector>

namespace Xna {
	struct Game::Implementation {
		//Raised when the game gains focus.
		CSharp::Event<CSharp::EventArgs> Activated;
		//Raised when the game loses focus.
		CSharp::Event<CSharp::EventArgs> Deactivated;
		//Raised when the game is being disposed.
		CSharp::Event<CSharp::EventArgs> Disposed;
		//Raised when the game is exiting.
		CSharp::Event<CSharp::EventArgs> Exiting;

		friend class Game;

		Xna::LaunchParameters launchParameters;
		GameComponentCollection gameComponents;

		std::optional<ContentManager> content;

		GameClock clock;
		CSharp::TimeSpan totalGameTime{ CSharp::TimeSpan::Zero() };
		CSharp::TimeSpan accumulatedElapsedGameTime{ CSharp::TimeSpan::Zero() };
		CSharp::TimeSpan lastFrameElapsedGameTime{ CSharp::TimeSpan::Zero() };
		CSharp::TimeSpan targetElapsedTime{ CSharp::TimeSpan::FromTicks(166667L) };
		CSharp::TimeSpan inactiveSleepTime{ CSharp::TimeSpan::FromMilliseconds(20.0) };
		
		GameServiceContainer gameServices{};

		bool isRun{ false };
		bool isRunnningSlowly{ false };
		bool doneFirstUpdate{ false };
		bool doneFirstDraw{ false };
		bool endRunRequired{ false };
		bool isActive{ false };
		bool forceElapsedTimeToZero{ false };
		bool isFixedTimeStep{ true };
		bool drawRunningSlowly{ false };
		bool suppressDraw{ false };
		bool exitRequest{ false };
		bool inRun{ false };
		bool isMouseVisible{ false };
		int32_t updatesSinceRunningSlowly1{ 0 };
		int32_t updatesSinceRunningSlowly2{ 0 };

		static constexpr CSharp::TimeSpan maximumElapsedTime = CSharp::TimeSpan::FromMilliseconds(500.0);

		Xna::GameTime gameTime;

		std::shared_ptr<IGraphicsDeviceManager> graphicsDeviceManager;
		std::shared_ptr<IGraphicsDeviceService> graphicsDeviceService;		

		std::optional<GameHost> host;

		std::vector<std::shared_ptr<IGameComponent>> notYetInitialized;
		
		std::vector<std::shared_ptr<IUpdateable>> updateableComponents;
		std::vector<std::shared_ptr<IDrawable>> drawableComponents;
		std::vector<std::shared_ptr<IDrawable>> currentlyDrawingComponents;
	};	
}

#endif