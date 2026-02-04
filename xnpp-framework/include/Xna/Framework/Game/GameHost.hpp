#ifndef XNA_FRAMEWORK_GAME_GAMEHOST_HPP
#define XNA_FRAMEWORK_GAME_GAMEHOST_HPP

#include "GameWindow.hpp"
#include <memory>
#include <optional>
#include "_Game.hpp"
#include "Xna/CSharp/Event.hpp"

namespace Xna {
	class GameHost {
	public:
		GameHost(Game const& game);

		GameWindow Window() const;
		void Run();
		void RunOneFrame();
		void StartGameLoop();
		void Exit();

		CSharp::Event<CSharp::EventArgs> Suspend() const;
		CSharp::Event<CSharp::EventArgs> Resume() const;
		CSharp::Event<CSharp::EventArgs> Activated() const;
		CSharp::Event<CSharp::EventArgs> Deactivated() const;
		CSharp::Event<CSharp::EventArgs> Idle() const;
		CSharp::Event<CSharp::EventArgs> Exiting() const;

	protected:
		void OnSuspend();
		void OnResume();
		void OnActivated();
		void OnDeactivated();
		void OnIdle();
		void OnExiting();

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