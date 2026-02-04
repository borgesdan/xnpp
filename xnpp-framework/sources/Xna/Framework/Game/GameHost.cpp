#include "Xna/Framework/Game/GameHost.hpp"
#include "Xna/Framework/Game/Game.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	GameHost::GameHost(Game const& game) {
		impl = std::make_shared<GameHost::Implementation>();
		impl->game = game;
		impl->gameWindow = GameWindow();		
	}

	CSharp::Event<CSharp::EventArgs> GameHost::Suspend() const { return impl->Suspend; }
	CSharp::Event<CSharp::EventArgs> GameHost::Resume() const { return impl->Resume; }
	CSharp::Event<CSharp::EventArgs> GameHost::Activated() const { return impl->Activated; }
	CSharp::Event<CSharp::EventArgs> GameHost::Deactivated() const { return impl->Deactivated; }
	CSharp::Event<CSharp::EventArgs> GameHost::Idle() const { return impl->Idle; }
	CSharp::Event<CSharp::EventArgs> GameHost::Exiting() const { return impl->Exiting; }

	void GameHost::OnSuspend() {
		impl->Suspend.Invoke(this, CSharp::EventArgs::Empty);
	}

	void GameHost::OnResume() {
		impl->Resume.Invoke(this, CSharp::EventArgs::Empty);
	}

	void GameHost::OnActivated() {
		impl->Activated.Invoke(this, CSharp::EventArgs::Empty);
	}

	void GameHost::OnDeactivated() {
		impl->Deactivated.Invoke(this, CSharp::EventArgs::Empty);
	}

	void GameHost::OnIdle() {
		impl->Idle.Invoke(this, CSharp::EventArgs::Empty);
	}

	void GameHost::OnExiting() {
		impl->Exiting.Invoke(this, CSharp::EventArgs::Empty);
	}

	GameWindow GameHost::Window() const { return impl->gameWindow; }

	void GameHost::Run() {
		if (impl->doneRun)
			throw CSharp::InvalidOperationException("NoMultipleRuns");

		impl->gameWindow.Activated().Add([this](void* sender, CSharp::EventArgs const& args) {
			this->OnActivated();
			});

		impl->gameWindow.Deactivated().Add([this](void* sender, CSharp::EventArgs const& args) {
			this->OnDeactivated();
			});

		impl->gameWindow.Create();

		auto graphicsDevice = impl->game->GraphicsDevice();
		//Uma inicialização tardia pois a janela foi criada neste momento
		//e o dispositivo gráfico foi criado sem o swapChain.
		Platform::GraphicsDevice_LazyInitialization(graphicsDevice, impl->gameWindow.Handle());

		Platform::GameHost_Tick(*this);

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