#include "Xna/Framework/Game/GameHost.hpp"
#include "Xna/Framework/Game/Game.hpp"
#include "Xna/Platform/Platform.hpp"
#include <Xna/CSharp/Exception.hpp>

namespace Xna {
	GameHost::GameHost(Game* game) {
		impl = std::make_shared<GameHost::Implementation>();
		impl->game = game;
		impl->gameWindow = GameWindow();		
	}

	IEvent<EventArgs>& GameHost::Suspend() const { return impl->Suspend; }
	IEvent<EventArgs>& GameHost::Resume() const { return impl->Resume; }
	IEvent<EventArgs>& GameHost::Activated() const { return impl->Activated; }
	IEvent<EventArgs>& GameHost::Deactivated() const { return impl->Deactivated; }
	IEvent<EventArgs>& GameHost::Idle() const { return impl->Idle; }
	IEvent<EventArgs>& GameHost::Exiting() const { return impl->Exiting; }

	void GameHost::OnSuspend() {
		impl->Suspend.Invoke(this, EventArgs::Empty());
	}

	void GameHost::OnResume() {
		impl->Resume.Invoke(this, EventArgs::Empty());
	}

	void GameHost::OnActivated() {
		impl->Activated.Invoke(this, EventArgs::Empty());
	}

	void GameHost::OnDeactivated() {
		impl->Deactivated.Invoke(this, EventArgs::Empty());
	}

	void GameHost::OnIdle() {
		impl->Idle.Invoke(this, EventArgs::Empty());
	}

	void GameHost::OnExiting() {
		impl->Exiting.Invoke(this, EventArgs::Empty());
	}

	GameWindow* GameHost::Window() const { return &impl->gameWindow; }

	void GameHost::Run() {
		if (impl->doneRun)
			throw CSharp::InvalidOperationException("NoMultipleRuns");

		impl->gameWindow.Activated().Add([this](void* sender, EventArgs const& args) {
			this->OnActivated();
			});

		impl->gameWindow.Deactivated().Add([this](void* sender, EventArgs const& args) {
			this->OnDeactivated();
			});

		impl->gameWindow.Create();		

		Platform::Host::Tick(*this);

		impl->doneRun = true;
		OnExiting();
	}

	void GameHost::RunOneFrame()
	{
		impl->gameWindow.Tick();
		OnIdle();
		//this.gameWindow.IsGuideVisible = Guide.IsVisible;
	}

	void GameHost::StartGameLoop() {}
	void GameHost::Exit() {
		impl->exitRequested = true;
	}
}