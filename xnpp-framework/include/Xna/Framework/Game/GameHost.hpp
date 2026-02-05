#ifndef XNA_FRAMEWORK_GAME_GAMEHOST_HPP
#define XNA_FRAMEWORK_GAME_GAMEHOST_HPP

#include "GameWindow.hpp"
#include <memory>
#include <optional>
#include "_Game.hpp"
#include "Xna/CSharp/Event.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class GameHost {
	public:
		XNPP_API GameHost(Game const& game);

		XNPP_API GameWindow Window() const;
		XNPP_API void Run();
		XNPP_API void RunOneFrame();
		XNPP_API void StartGameLoop();
		XNPP_API void Exit();

		XNPP_API CSharp::Event<CSharp::EventArgs> Suspend() const;
		XNPP_API CSharp::Event<CSharp::EventArgs> Resume() const;
		XNPP_API CSharp::Event<CSharp::EventArgs> Activated() const;
		XNPP_API CSharp::Event<CSharp::EventArgs> Deactivated() const;
		XNPP_API CSharp::Event<CSharp::EventArgs> Idle() const;
		XNPP_API CSharp::Event<CSharp::EventArgs> Exiting() const;

	protected:
		XNPP_API void OnSuspend();
		XNPP_API void OnResume();
		XNPP_API void OnActivated();
		XNPP_API void OnDeactivated();
		XNPP_API void OnIdle();
		XNPP_API void OnExiting();

	private:
		struct Implementation {
			std::optional<Game> game;
			bool doneRun{ false };
			bool exitRequested{ false };
			GameWindow gameWindow{ nullptr };

			CSharp::Event<CSharp::EventArgs> Suspend;
			CSharp::Event<CSharp::EventArgs> Resume;
			CSharp::Event<CSharp::EventArgs> Activated;
			CSharp::Event<CSharp::EventArgs> Deactivated;
			CSharp::Event<CSharp::EventArgs> Idle;
			CSharp::Event<CSharp::EventArgs> Exiting;
		};
		
		std::shared_ptr<Implementation> impl;

		friend struct Platform;
	};

}

#endif