#include <Xna/Framework/Content/ContentManager.hpp>
#include <Xna/Framework/FrameworkDispatcher.hpp>
#include <Xna/Framework/Game/DrawableGameComponent.hpp>
#include <Xna/Framework/Game/Game.hpp>
#include <Xna/Framework/Game/GameClock.hpp>
#include <Xna/Framework/Game/GameHost.hpp>
#include <Xna/Framework/Game/GameServiceContainer.hpp>
#include <Xna/Framework/Game/GameTime.hpp>
#include <Xna/Framework/Game/GameWindow.hpp>
#include <Xna/Framework/Game/GraphicsDeviceManager.hpp>
#include <Xna/Framework/Game/IGameComponent.hpp>
#include <Xna/Framework/Game/IGraphicsDeviceManager.hpp>
#include <Xna/Framework/Game/LaunchParameters.hpp>
#include <Xna/Framework/Graphics/IGraphicsDeviceService.hpp>
#include <Xna/Internal/App.hpp>
#include <Xna/Platform/Platform.hpp>
#include <exception>
#include <limits>
#include <vector>
#include <Xna/CSharp/Thread.hpp>
#include <Xna/Framework/Game/GameComponentCollection.hpp>
#include <Xna/Macros/CoreAssert.hpp>

using Xna::TimeSpan;

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

		GameClock clock;
		TimeSpan totalGameTime{ TimeSpan::Zero() };
		TimeSpan accumulatedElapsedGameTime{ TimeSpan::Zero() };
		TimeSpan lastFrameElapsedGameTime{ TimeSpan::Zero() };
		TimeSpan targetElapsedTime{ TimeSpan::FromTicks(166667L) };
		TimeSpan inactiveSleepTime{ TimeSpan::FromMilliseconds(20.0) };

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

		static constexpr TimeSpan maximumElapsedTime = TimeSpan::FromMilliseconds(500.0);

		Xna::GameTime gameTime;

		std::shared_ptr<IGraphicsDeviceManager> graphicsDeviceManager;
		std::shared_ptr<IGraphicsDeviceService> graphicsDeviceService;
		std::vector<std::shared_ptr<IGameComponent>> notYetInitialized;
		std::vector<std::shared_ptr<IUpdateable>> updateableComponents;
		std::vector<std::shared_ptr<IDrawable>> drawableComponents;
		std::vector<std::shared_ptr<IDrawable>> currentlyDrawingComponents;

		std::unique_ptr<GameHost> host;
		std::unique_ptr<ContentManager> content;
	};

	Game::Game() {
		Log::Init();
		
		impl = std::make_unique<Implementation>();
		impl->content = std::make_unique<ContentManager>(impl->gameServices, "");
		impl->host = std::make_unique<GameHost>(this);

		EnsureHost();

		impl->gameComponents.ComponentAdded().Add([this](void* sender, GameComponentCollectionEventArgs const& e) {
			GameComponentAdded(sender, e);
			});

		impl->gameComponents.ComponentRemoved().Add([this](void* sender, GameComponentCollectionEventArgs const& e) {
			GameComponentRemoved(sender, e);
			});
	}

	Game::~Game() {
		Log::Shutdown();
	}

	GameComponentCollection Game::Components() const { return impl->gameComponents; }
	ContentManager* Game::Content() const { return impl->content.get(); }
	Xna::LaunchParameters Game::LaunchParameters() const { return impl->launchParameters; }
	void Game::LaunchParameters(Xna::LaunchParameters const& value) { impl->launchParameters = value; }
	GameServiceContainer Game::Services() const { return impl->gameServices; }

	CSharp::Event<CSharp::EventArgs> Game::Activated() const { return impl->Activated; }
	CSharp::Event<CSharp::EventArgs> Game::Deactivated() const { return impl->Deactivated; }
	CSharp::Event<CSharp::EventArgs> Game::Disposed() const { return impl->Disposed; }
	CSharp::Event<CSharp::EventArgs> Game::Exiting() const { return impl->Exiting; }

	void Game::OnActivated(void* sender, CSharp::EventArgs args) {
		impl->Activated.Invoke(this, args);
	}

	void Game::OnDeactivated(void* sender, CSharp::EventArgs args) {
		impl->Deactivated.Invoke(this, args);
	}

	void Game::OnExiting(void* sender, CSharp::EventArgs args) {
		impl->Exiting.Invoke(this, args);
	}

	void Game::Run() {
		try {
			XNPP_ENGINE_INFO("Starting game loop.");

			XNPP_ENGINE_INFO("Registering content type readers.");
			App::RegisterContentTypeReaders();

			XNPP_ENGINE_INFO("Initializing platform system.");
			Platform::System::Initialize();

			XNPP_ENGINE_INFO("Running game.");
			RunGame();
		}
		catch (std::exception& ex) {
			auto _ex = dynamic_cast<CSharp::Exception*>(&ex);

			std::string message = "Game::Run() - Unhandled exception: ";

			if (_ex == nullptr) {
				message += ex.what();
			}
			else {
#if DEBUG || _DEBUG
				message += _ex->FullMessage();
#else
				message += _ex->Message();
#endif
			}

			XNPP_ENGINE_CRITICAL(message);
		}

		Platform::System::Dispose();
	}

	void Game::InternalRunGame() {
		Initialize();
		impl->isRun = true;
		BeginRun();
		impl->gameTime.ElapsedGameTime(TimeSpan::Zero());
		impl->gameTime.TotalGameTime(impl->totalGameTime);
		impl->isRunnningSlowly = false;
		Update(impl->gameTime);
		impl->doneFirstUpdate = true;
	}

	void Game::RunGame() {
		impl->graphicsDeviceManager = impl->gameServices.GetService<IGraphicsDeviceManager>();

		XNPP_CORE_ASSERT(impl->graphicsDeviceManager != nullptr, "No IGraphicsDeviceManager was found in the GameServiceContainer. Please add one before calling Game::Run().");		

		if (impl->host)
			impl->host->Run();
		else 
			XNPP_ENGINE_WARN("No GameHost was found.");

		EndRun();
	}

	void Game::RunOneFrame() const {
		if (!impl->host)
			return;

		impl->host->RunOneFrame();
	}

	void Game::Tick() {
		if (ShouldExit())
			return;

		if (!impl->isActive)
			CSharp::Thread::Sleep(static_cast<long long>(impl->inactiveSleepTime.TotalMilliseconds()));

		impl->clock.Step();

		bool flag = true;
		auto timeSpan1 = impl->clock.ElapsedAdjustedTime();

		if (timeSpan1 < TimeSpan::Zero())
			timeSpan1 = TimeSpan::Zero();

		if (impl->forceElapsedTimeToZero) {
			timeSpan1 = TimeSpan::Zero();
			impl->forceElapsedTimeToZero = false;
		}

		if (timeSpan1 > impl->maximumElapsedTime)
			timeSpan1 = impl->maximumElapsedTime;

		if (impl->isFixedTimeStep)
		{
			const auto _ticks = timeSpan1.Ticks();
			const auto _elapsedTicks = impl->targetElapsedTime.Ticks();
			const auto _absTicks = std::abs(_ticks - _elapsedTicks);
			const auto _elapsedTicks6 = impl->targetElapsedTime.Ticks() >> 6;

			if (_absTicks < _elapsedTicks6)
				timeSpan1 = impl->targetElapsedTime;

			impl->accumulatedElapsedGameTime = impl->accumulatedElapsedGameTime.Add(timeSpan1);

			auto num = impl->accumulatedElapsedGameTime.Ticks() / impl->targetElapsedTime.Ticks();
			impl->accumulatedElapsedGameTime = TimeSpan::FromTicks(impl->accumulatedElapsedGameTime.Ticks() % impl->targetElapsedTime.Ticks());

			impl->lastFrameElapsedGameTime = TimeSpan::Zero();

			if (num == 0L)
				return;

			TimeSpan targetElapsedTime = impl->targetElapsedTime;

			if (num > 1L)
			{
				impl->updatesSinceRunningSlowly2 = impl->updatesSinceRunningSlowly1;
				impl->updatesSinceRunningSlowly1 = 0;
			}
			else
			{
				if (impl->updatesSinceRunningSlowly1 < std::numeric_limits<int32_t>::max())
					++impl->updatesSinceRunningSlowly1;
				if (impl->updatesSinceRunningSlowly2 < std::numeric_limits<int32_t>::max())
					++impl->updatesSinceRunningSlowly2;
			}

			impl->drawRunningSlowly = impl->updatesSinceRunningSlowly2 < 20;

			//TODO: FrameworkDispatcher::Update() original fica dentor do bloco Update(GameTime gameTime)
			FrameworkDispatcher::Update();

			while (num > 0L && !ShouldExit())
			{
				--num;
				try
				{
					impl->gameTime.ElapsedGameTime(targetElapsedTime);
					impl->gameTime.TotalGameTime(impl->totalGameTime);
					impl->gameTime.IsRunningSlowly(impl->drawRunningSlowly);
					Update(impl->gameTime);

					flag &= impl->suppressDraw;
					impl->suppressDraw = false;
				}
				catch (std::exception& ex) {
					//TODO: fazer alguma coisa aqui?
				}

				impl->lastFrameElapsedGameTime = impl->lastFrameElapsedGameTime.Add(targetElapsedTime);
				impl->totalGameTime = impl->totalGameTime.Add(targetElapsedTime);
			}
		}
		else
		{
			auto timeSpan2 = timeSpan1;
			impl->drawRunningSlowly = false;
			impl->updatesSinceRunningSlowly1 = std::numeric_limits<int32_t>::max();
			impl->updatesSinceRunningSlowly2 = std::numeric_limits<int32_t>::max();

			if (!ShouldExit())
			{
				try
				{
					impl->lastFrameElapsedGameTime = timeSpan2;
					impl->gameTime.ElapsedGameTime(impl->lastFrameElapsedGameTime);
					impl->gameTime.TotalGameTime(impl->totalGameTime);
					impl->gameTime.IsRunningSlowly(false);
					Update(impl->gameTime);
					flag &= impl->suppressDraw;
					impl->suppressDraw = false;
				}
				catch (std::exception& ex)
				{

				}

				impl->totalGameTime.Add(timeSpan2);
			}
		}

		if (flag)
			return;

		DrawFrame();
	}

	void Game::SuppressDraw() const {
		impl->suppressDraw = true;
	}

	void Game::Exit() {
		impl->exitRequest = true;
		impl->host->Exit();

		if (!impl->inRun || !impl->endRunRequired)
			return;

		EndRun();
		impl->isRun = false;
	}

	void Game::Update(GameTime& gameTime) {
		auto& updateableComponents = impl->updateableComponents;

		for (size_t index = 0; index < updateableComponents.size(); ++index) {
			auto& updatingComponent = updateableComponents[index];
			if (updatingComponent && updatingComponent->Enabled())
				updatingComponent->Update(gameTime);
		}

		impl->doneFirstUpdate = true;

		//Movido para o Tick()
		//FrameworkDispatcher::Update();
		Platform::System::Update();
	}

	bool Game::BeginDraw() {
		return impl->graphicsDeviceManager == nullptr || impl->graphicsDeviceManager->BeginDraw();
	}

	void Game::Draw(GameTime& gameTime) {
		auto& drawableComponents = impl->drawableComponents;

		for (size_t index = 0; index < drawableComponents.size(); ++index) {
			auto& drawingComponent = drawableComponents[index];
			if (drawingComponent && drawingComponent->Visible())
				drawingComponent->Draw(gameTime);
		}
	}

	void Game::EndDraw() {
		if (impl->graphicsDeviceManager == nullptr)
			return;

		impl->graphicsDeviceManager->EndDraw();
	}

	void Game::Paint() {
		if (!impl->doneFirstDraw)
			return;

		DrawFrame();
	}

	void Game::Initialize() {
		HookDeviceEvents();

		for (size_t i = 0; i < impl->notYetInitialized.size(); ++i) {
			if (impl->notYetInitialized[i] == nullptr)
				continue;

			impl->notYetInitialized[i]->Initialize();
		}
		impl->notYetInitialized.clear();

		//TODO: this.graphicsDeviceService.GraphicsDevice == null
		if (impl->graphicsDeviceService == nullptr)
			return;

		LoadContent();
	}

	void Game::ResetElapsedTime() {
		impl->forceElapsedTimeToZero = true;
		impl->drawRunningSlowly = false;
		impl->updatesSinceRunningSlowly1 = std::numeric_limits<int32_t>::max();
		impl->updatesSinceRunningSlowly2 = std::numeric_limits<int32_t>::max();
	}

	void Game::DrawFrame() {
		try
		{
			if (ShouldExit() || !impl->doneFirstUpdate || (Window()->IsMinimized)() || !BeginDraw())
				return;

			impl->gameTime.TotalGameTime(impl->totalGameTime);
			impl->gameTime.ElapsedGameTime(impl->lastFrameElapsedGameTime);
			impl->gameTime.IsRunningSlowly(impl->drawRunningSlowly);
			Draw(impl->gameTime);
			EndDraw();
			impl->doneFirstDraw = true;
		}
		catch (std::exception& e)
		{

		}
		impl->lastFrameElapsedGameTime = TimeSpan::Zero();
	}

	void Game::EnsureHost() {
		impl->host->Activated().Add([this](void* sender, CSharp::EventArgs const& args) {
			HostActivated(sender, args);
			});
		impl->host->Deactivated().Add([this](void* sender, CSharp::EventArgs const& args) {
			HostDeactivated(sender, args);
			});
		impl->host->Suspend().Add([this](void* sender, CSharp::EventArgs const& args) {
			HostSuspend(sender, args);
			});
		impl->host->Resume().Add([this](void* sender, CSharp::EventArgs const& args) {
			HostResume(sender, args);
			});
		impl->host->Idle().Add([this](void* sender, CSharp::EventArgs const& args) {
			HostIdle(sender, args);
			});
		impl->host->Exiting().Add([this](void* sender, CSharp::EventArgs const& args) {
			HostExiting(sender, args);
			});
	}

	void Game::HostSuspend(void* sender, CSharp::EventArgs const& e) {
		impl->clock.Suspend();
	}

	void Game::HostResume(void* sender, CSharp::EventArgs const& e) {
		impl->clock.Resume();
	}

	void Game::HostExiting(void* sender, CSharp::EventArgs const& e) {
		OnExiting(sender, CSharp::EventArgs::Empty());
	}

	void Game::HostIdle(void* sender, CSharp::EventArgs const& e) {
		Tick();
	}

	void Game::HostDeactivated(void* sender, CSharp::EventArgs const& e) {
		if (!impl->isActive)
			return;

		impl->isActive = false;
		OnDeactivated(this, CSharp::EventArgs::Empty());
	}

	void Game::HostActivated(void* sender, CSharp::EventArgs const& e) {
		if (impl->isActive)
			return;

		if (impl->graphicsDeviceManager != nullptr)
			impl->graphicsDeviceManager->CreateDevice();

		InternalRunGame();

		impl->isActive = true;
		OnActivated(this, CSharp::EventArgs::Empty());
	}

	void Game::HookDeviceEvents() {
		impl->graphicsDeviceService = Services().GetService<IGraphicsDeviceService>();

		if (impl->graphicsDeviceService == nullptr)
			return;

		impl->graphicsDeviceService->DeviceCreated().Add([this](void* sender, CSharp::EventArgs const& args) {
			DeviceCreated(sender, args);
			});
		impl->graphicsDeviceService->DeviceResetting().Add([this](void* sender, CSharp::EventArgs const& args) {
			DeviceResetting(sender, args);
			});
		impl->graphicsDeviceService->DeviceReset().Add([this](void* sender, CSharp::EventArgs const& args) {
			DeviceReset(sender, args);
			});
	}

	void UnhookDeviceEvents() {
		//TODO::
	}

	void Game::DeviceResetting(void* sender, CSharp::EventArgs const& e) {

	}

	void Game::DeviceReset(void* sender, CSharp::EventArgs const& e) {

	}

	void Game::DeviceCreated(void* sender, CSharp::EventArgs const& e) {
		LoadContent();
	}

	bool Game::ShouldExit() {
		return impl->exitRequest;
	}

	TimeSpan Game::InactiveSleepTime() const { return impl->inactiveSleepTime; }
	void Game::InactiveSleepTime(TimeSpan const& value) {
		if (!(value < TimeSpan::Zero()))
			impl->inactiveSleepTime = TimeSpan::Zero();
		else
			impl->inactiveSleepTime = value;
	}

	bool Game::IsMouseVisible() const { return impl->isMouseVisible; }
	void Game::IsMouseVisible(bool value) {
		impl->isMouseVisible = value;

		if (!Window().has_value())
			return;

		Window()->IsMouseVisible(value);
	}

	TimeSpan Game::TargetElapsedTime() const { return impl->targetElapsedTime; }
	void Game::TargetElapsedTime(TimeSpan const& value) {
		if (!(value <= TimeSpan::Zero()))
			throw CSharp::ArgumentOutOfRangeException();

		impl->targetElapsedTime = value;
	}

	bool Game::IsFixedTimeStep() const {
		return impl->isFixedTimeStep;
	}

	void Game::IsFixedTimeStep(bool value) {
		impl->isFixedTimeStep = value;
	}

	std::optional<GameWindow> Game::Window() const {
		if (impl->host)
			return impl->host->Window();

		return std::nullopt;
	}

	bool Game::IsActive() const {
		//TODO:
		return true;
	}

	Xna::GraphicsDevice Game::GraphicsDevice() const {
		if (impl->graphicsDeviceService == nullptr)
			impl->graphicsDeviceService = Services().GetService<IGraphicsDeviceService>();

		if (impl->graphicsDeviceService == nullptr)
			throw CSharp::InvalidOperationException("IGraphicsDeviceService is null.");

		if (!impl->graphicsDeviceService->GraphicsDevice().has_value())
			throw CSharp::InvalidOperationException("GraphicsDevice is null.");

		return impl->graphicsDeviceService->GraphicsDevice().value();
	}

	void Game::GameComponentAdded(void* sender, GameComponentCollectionEventArgs e) {
		if (impl->inRun)
			e.GameComponent()->Initialize();
		else
			impl->notYetInitialized.push_back(e.GameComponent());

		const auto gc = e.GameComponent();
		const auto updateable = dynamic_pointer_cast<IUpdateable>(gc);

		if (updateable) {
			// Busca a posição correta usando lower_bound (equivalente ao BinarySearch)
			//int num = this.updateableComponents.BinarySearch(gameComponent1, (IComparer<IUpdateable>) UpdateOrderComparer.Default);
			auto it = std::lower_bound(
				impl->updateableComponents.begin(),
				impl->updateableComponents.end(),
				updateable,
				[](const std::shared_ptr<IUpdateable>& a, const std::shared_ptr<IUpdateable>& b) {
					return a->UpdateOrder() < b->UpdateOrder();
				});

			// O XNA usa o loop while para garantir que o novo componente fique 
			// após outros com o mesmo UpdateOrder
			while (it != impl->updateableComponents.end() && (*it)->UpdateOrder() == updateable->UpdateOrder()) {
				++it;
			}

			impl->updateableComponents.insert(it, updateable);
			//TODO: não implementado
			//gameComponent1.UpdateOrderChanged += new EventHandler<EventArgs>(this.UpdateableUpdateOrderChanged);
		}

		const auto drawable = dynamic_pointer_cast<IDrawable>(gc);

		if (!drawable)
			return;

		auto it = std::lower_bound(
			impl->drawableComponents.begin(),
			impl->drawableComponents.end(),
			drawable,
			[](const std::shared_ptr<IDrawable>& a, const std::shared_ptr<IDrawable>& b) {
				return a->DrawOrder() < b->DrawOrder();
			});

		while (it != impl->drawableComponents.end() && (*it)->DrawOrder() == drawable->DrawOrder()) {
			++it;
		}

		impl->drawableComponents.insert(it, drawable);

		//TODO: não implementado
		//gameComponent2.DrawOrderChanged += new EventHandler<EventArgs>(this.DrawableDrawOrderChanged);
	}

	void Game::GameComponentRemoved(void* sender, GameComponentCollectionEventArgs e) {
		if (!impl->inRun)
		{
			auto it = std::find(impl->notYetInitialized.begin(), impl->notYetInitialized.end(), e.GameComponent());
			impl->notYetInitialized.erase(it);
		}
		const auto gc = e.GameComponent();
		const auto updateable = dynamic_pointer_cast<IUpdateable>(gc);

		if (updateable) {
			auto it = std::find(impl->updateableComponents.begin(), impl->updateableComponents.end(), updateable);
			impl->updateableComponents.erase(it);
			//TODO: não implementado
			//gameComponent1.UpdateOrderChanged -= new EventHandler<EventArgs>(this.UpdateableUpdateOrderChanged);
		}

		const auto drawable = dynamic_pointer_cast<IDrawable>(gc);

		if (!drawable)
			return;

		auto it = std::find(impl->drawableComponents.begin(), impl->drawableComponents.end(), drawable);
		impl->drawableComponents.erase(it);
		//TODO: não implementado
		//gameComponent2.DrawOrderChanged -= new EventHandler<EventArgs>(this.DrawableDrawOrderChanged);
	}
}