#pragma once

#include "Xna/Framework/Game/GameWindow.hpp"
#include "Xna/Macros/Api.hpp"
#include "Xna/Macros/CoreAssert.hpp"
#include <Xna/CSharp/Event.hpp>

namespace Xna {
	class GameHost {
	public:
		virtual ~GameHost() = default;
		virtual void Tick() = 0;
		virtual GameWindow* Window() const noexcept = 0;
		virtual void Run() {
			XNPP_CORE_ASSERT(!doneRun, "A função Run já foi chamada anteriormente.");	
			XNPP_ENGINE_INFO("Executando Host.");

			Window()->Activated().Add([this](void* sender, EventArgs args) { OnActivated();	});
			Window()->Deactivated().Add([this](void* sender, EventArgs args) { OnDeactivated(); });

			Window()->Create();

			Tick();

			doneRun = true;
			OnExiting();
		}
		
		virtual void RunOneFrame() {
			Window()->Tick();
			OnIdle();
		}

		virtual void StartGameLoop() {}
		virtual void Exit() noexcept { exitRequested = true; }

		[[nodiscard]] IEvent<EventArgs>& Suspend() noexcept { return suspend; }
		[[nodiscard]] IEvent<EventArgs>& Resume() noexcept { return resume; }
		[[nodiscard]] IEvent<EventArgs>& Activated() noexcept { return activated; }
		[[nodiscard]] IEvent<EventArgs>& Deactivated() noexcept { return deactivated; }
		[[nodiscard]] IEvent<EventArgs>& Idle() noexcept { return idle; }
		[[nodiscard]] IEvent<EventArgs>& Exiting() noexcept { return exiting; }

	protected:
		virtual void OnSuspend() { suspend.Invoke(this, EventArgs::Empty()); }
		virtual void OnResume() { resume.Invoke(this, EventArgs::Empty()); }
		virtual void OnActivated() { activated.Invoke(this, EventArgs::Empty()); }
		virtual void OnDeactivated() { deactivated.Invoke(this, EventArgs::Empty()); }
		virtual void OnIdle() { idle.Invoke(this, EventArgs::Empty()); }
		virtual void OnExiting() { exiting.Invoke(this, EventArgs::Empty()); }
		[[nodiscard]] bool ExitRequested() const noexcept { return exitRequested; }

	private:
		Event<EventArgs> suspend;
		Event<EventArgs> resume;
		Event<EventArgs> activated;
		Event<EventArgs> deactivated;
		Event<EventArgs> idle;
		Event<EventArgs> exiting;
		bool exitRequested{ false };
		bool doneRun{ false };
	};
}
