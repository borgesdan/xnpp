#ifndef XNA_FRAMEWORK_GAME_GAMEHOST_HPP
#define XNA_FRAMEWORK_GAME_GAMEHOST_HPP

#include "GameWindow.hpp"
#include <memory>
#include <optional>
#include "Game.hpp"
#include <Xna/CSharp/Event.hpp>
#include "Xna/Macros/Api.hpp"

namespace Xna {
	class GameHost {
	public:
		XNPP_API GameHost(Game* game);

		XNPP_API GameWindow* Window() const;
		XNPP_API void Run();
		XNPP_API void RunOneFrame();
		XNPP_API void StartGameLoop();
		XNPP_API void Exit();

		XNPP_API IEvent<EventArgs>& Suspend() const;
		XNPP_API IEvent<EventArgs>& Resume() const;
		XNPP_API IEvent<EventArgs>& Activated() const;
		XNPP_API IEvent<EventArgs>& Deactivated() const;
		XNPP_API IEvent<EventArgs>& Idle() const;
		XNPP_API IEvent<EventArgs>& Exiting() const;

	protected:
		XNPP_API void OnSuspend();
		XNPP_API void OnResume();
		XNPP_API void OnActivated();
		XNPP_API void OnDeactivated();
		XNPP_API void OnIdle();
		XNPP_API void OnExiting();

	private:
		struct Implementation {
			Xna::Game* game;
			bool doneRun{ false };
			bool exitRequested{ false };
			GameWindow gameWindow{ nullptr };

			Event<EventArgs> Suspend;
			Event<EventArgs> Resume;
			Event<EventArgs> Activated;
			Event<EventArgs> Deactivated;
			Event<EventArgs> Idle;
			Event<EventArgs> Exiting;
		};
		
		std::shared_ptr<Implementation> impl;

		friend struct Platform::Host;
	};

}

#endif