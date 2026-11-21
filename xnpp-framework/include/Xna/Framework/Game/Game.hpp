#pragma once

#include "Xna/Macros/Api.hpp"
#include "Xna/Macros/Utils.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <Xna/CSharp/Collection.hpp>
#include <Xna/CSharp/Event.hpp>
#include <Xna/CSharp/TimeSpan.hpp>

namespace Xna {	
	class ContentManager;
	class GraphicsDevice;
	class GameServiceContainer;
	class GameWindow;

	class Game;

	//////////////////////////////////////////////////////////////////
	// GAME CLOCK													//
	//////////////////////////////////////////////////////////////////

	struct GameClock {
	public:
		inline GameClock() { Reset(); }

		constexpr TimeSpan CurrentTime() const { return currentTimeBase + currentTimeOffset; }
		constexpr TimeSpan ElapsedTime() const { return elapsedTime; }
		constexpr TimeSpan ElapsedAdjustedTime() const { return elapsedAdjustedTime; }

		XNPP_API void Reset();
		XNPP_API void Step();
		XNPP_API void Suspend();
		XNPP_API void Resume();

		XNPP_API static int64_t Counter();
		XNPP_API static int64_t Frequency();

		XNPP_API static TimeSpan CounterToTimeSpan(int64_t delta);

	private:
		int64_t baseRealTime{ 0 };
		int64_t lastRealTime{ 0 };
		bool lastRealTimeValid{ false };
		int32_t suspendCount;
		int64_t suspendStartTime{ 0 };
		int64_t timeLostToSuspension{ 0 };
		TimeSpan currentTimeOffset;
		TimeSpan currentTimeBase;
		TimeSpan elapsedTime;
		TimeSpan elapsedAdjustedTime;
	};

	//////////////////////////////////////////////////////////////////
	// GAME TIME													//
	//////////////////////////////////////////////////////////////////

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
		bool IsRunningSlowly{ false };
		//The amount of game time since the start of the game.
		TimeSpan TotalGameTime{};
		//The amount of real time (wall clock) since the start of the game.
		TimeSpan TotalRealTime{};
	};

	//////////////////////////////////////////////////////////////////
	// IUPDATEABLE													//
	//////////////////////////////////////////////////////////////////

	//Defines an interface for a game component that should be updated in Game.Update.
	class IUpdateable {
	public:
		virtual ~IUpdateable() = default;
		//Indicates whether the game component's Update method should be called in Game.Update.
		virtual bool Enabled() const noexcept = 0;
		//Indicates when the game component should be updated relative to other game components. Lower values are updated first.
		virtual int UpdateOrder() const noexcept = 0;
		//Called when the game component should be updated.
		virtual void Update(GameTime& gameTime) = 0;
	};

	//////////////////////////////////////////////////////////////////
	// IDRAWABLE													//
	//////////////////////////////////////////////////////////////////

	//Defines the interface for a drawable game component.
	class IDrawable {
	public:
		virtual ~IDrawable() = default;
		//Indicates whether IDrawable.Draw should be called in Game.Draw for this game component.
		virtual bool Visible() const noexcept = 0;
		//The order in which to draw this object relative to other objects. Objects with a lower value are drawn first.
		virtual int DrawOrder() const noexcept = 0;
		//Draws the IDrawable.
		virtual void Draw(GameTime& gameTime) = 0;
	};

	//////////////////////////////////////////////////////////////////
	// IGAMECOMPONENT												//
	//////////////////////////////////////////////////////////////////

	//Defines an interface for game components.
	class IGameComponent {
	public:
		virtual ~IGameComponent() = 0;
		//Called when the component should be initialized. This method can be used for tasks like querying for services the component needs and setting up non-graphics resources.
		virtual void Initialize() = 0;
	};

	//////////////////////////////////////////////////////////////////
	// GAME COMPONENT												//
	//////////////////////////////////////////////////////////////////

	//A game component that is notified when it needs to draw itself.
	class GameComponent : public IGameComponent, public IUpdateable {
	public:
		//Initializes a new instance of this class.
		GameComponent(Xna::Game* game) : game(game) {}

		virtual ~GameComponent() override = default;

		//Indicates whether GameComponent.Update should be called when Game.Update is called.
		bool Enabled() const noexcept override { return enabled; }
		//Indicates whether GameComponent.Update should be called when Game.Update is called.
		void Enabled(bool value) { enabled = value; }
		//Indicates the order in which the GameComponent should be updated relative to other GameComponent instances. Lower values are updated first.
		int32_t UpdateOrder() const noexcept override { return updateOrder; }
		//Indicates the order in which the GameComponent should be updated relative to other GameComponent instances. Lower values are updated first.
		void UpdateOrder(int32_t value) { updateOrder = value; }
		//Gets the Game associated with this GameComponent.
		Xna::Game* Game() const { return game; }
		//Called when the GameComponent needs to be initialized.
		virtual void Initialize() override {}
		//Called when the GameComponent needs to be updated. Override this method with component-specific update code.
		virtual void Update(GameTime& gameTime) override {}

	private:
		bool enabled{ true };
		int32_t updateOrder{ 0 };
		Xna::Game* game;
	};

	//////////////////////////////////////////////////////////////////
	// DRAWABLE GAME COMPONENT										//
	//////////////////////////////////////////////////////////////////

	//A game component that is notified when it needs to draw itself.
	class DrawableGameComponent : public GameComponent, public IDrawable {
	public:
		//Creates a new instance of DrawableGameComponent.
		DrawableGameComponent(Xna::Game* game) : GameComponent(game) {}

		virtual ~DrawableGameComponent() override = default;

		//Order in which the component should be drawn, relative to other components that are in the same GameComponentCollection.
		int32_t DrawOrder() const noexcept override { return drawOrder; }
		//Order in which the component should be drawn, relative to other components that are in the same GameComponentCollection.
		void DrawOrder(int32_t value) { drawOrder = value; }
		//Indicates whether Draw should be called.
		bool Visible() const noexcept override { return visible; }
		//Indicates whether Draw should be called.
		void Visible(bool value) { visible = value; }

		//Called when the DrawableGameComponent needs to be drawn. Override this method with component-specific drawing code.
		virtual void Draw(GameTime& gameTime) {}
		// 	Initializes the component. Override this method to load any non-graphics resources and query for any required services.
		virtual void Initialize() override {
			//TODO: GameComponent::Initialize()
			LoadContent();
		}

	protected:
		//Called when graphics resources need to be loaded. Override this method to load any component-specific graphics resources.
		virtual void LoadContent() {}
		//Called when graphics resources need to be unloaded. Override this method to unload any component-specific graphics resources.
		virtual void UnloadContent() {}

	private:
		bool visible{ true };
		int32_t drawOrder{ 0 };
	};

	//////////////////////////////////////////////////////////////////
	// GAME COMPONENT COLLECTION									//
	//////////////////////////////////////////////////////////////////

	struct GameComponentCollectionEventArgs : public EventArgs {
	public:
		std::shared_ptr<IGameComponent> GameComponent() const { return gameComponent; }

		GameComponentCollectionEventArgs(std::shared_ptr<IGameComponent> gameComponent) :
			gameComponent(gameComponent) {}

	private:
		std::shared_ptr<IGameComponent> gameComponent;
	};

	//A collection of game components.
	class GameComponentCollection final : public CSharp::Collection<std::shared_ptr<IGameComponent>> {
	public:
		inline GameComponentCollection() {
			impl = std::make_shared<Implementation>();
		}

		~GameComponentCollection() override = default;

		IEvent<GameComponentCollectionEventArgs>& ComponentAdded() const { return impl->componentAdded; }
		IEvent<GameComponentCollectionEventArgs>& ComponentRemoved() const { return impl->componentRemoved; }

		inline GameComponentCollection(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(GameComponentCollection const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		XNPP_API void InsertItem(size_t index, std::shared_ptr<IGameComponent> const& item);
		XNPP_API void RemoveItem(size_t index) override;
		XNPP_API void SetItem(size_t index, std::shared_ptr<IGameComponent> const& item);
		XNPP_API void ClearItems() override;

	private:
		XNPP_API void OnComponentAdded(GameComponentCollectionEventArgs const& eventArgs);
		XNPP_API void OnComponentRemoved(GameComponentCollectionEventArgs const& eventArgs);

		struct Implementation {
			Event<GameComponentCollectionEventArgs> componentAdded;
			Event<GameComponentCollectionEventArgs> componentRemoved;
		};

		std::shared_ptr<Implementation> impl;
	};

	//////////////////////////////////////////////////////////////////
	// GAME															//
	//////////////////////////////////////////////////////////////////

	//Provides basic graphics device initialization, game logic, and rendering code.
	class Game {
	public:
		//Initializes a new instance of this class, which provides basic graphics device initialization, game logic, rendering code, and a game loop.
		XNPP_API Game();
		XNPP_API virtual ~Game();

		//Gets the collection of GameComponents owned by the game.
		XNPP_API [[nodiscard]] GameComponentCollection Components() const noexcept;
		//Gets the current ContentManager.
		XNPP_API [[nodiscard]] ContentManager* Content() const noexcept;
		//Gets the current GraphicsDevice.
		XNPP_API [[nodiscard]] Xna::GraphicsDevice* GraphicsDevice() const noexcept;
		//Gets or sets the time to sleep when the game is inactive.
		XNPP_API [[nodiscard]] TimeSpan InactiveSleepTime() const noexcept;
		//Gets or sets the time to sleep when the game is inactive.
		XNPP_API void InactiveSleepTime(TimeSpan value) noexcept;
		//Indicates whether the game is currently the active application.
		XNPP_API [[nodiscard]] bool IsActive() const noexcept;
		//Gets or sets a value indicating whether to use fixed time steps.
		XNPP_API [[nodiscard]] bool IsFixedTimeStep() const noexcept;
		//Gets or sets a value indicating whether to use fixed time steps.
		XNPP_API void IsFixedTimeStep(bool value) noexcept;
		//Gets or sets a value indicating whether the mouse cursor should be visible.
		XNPP_API [[nodiscard]] bool IsMouseVisible() const noexcept;
		//Gets or sets a value indicating whether the mouse cursor should be visible.
		XNPP_API void IsMouseVisible(bool value) noexcept;
		//Gets the GameServiceContainer holding all the service providers attached to the Game.
		XNPP_API [[nodiscard]] GameServiceContainer* Services() const noexcept;
		//Gets or sets the target time between calls to Update when IsFixedTimeStep is true.
		XNPP_API [[nodiscard]] TimeSpan TargetElapsedTime() const noexcept;
		//Gets or sets the target time between calls to Update when IsFixedTimeStep is true.
		XNPP_API void TargetElapsedTime(TimeSpan value) noexcept;
		//Gets the underlying operating system window.
		XNPP_API [[nodiscard]] GameWindow* Window() const noexcept;

		//Exits the game.
		XNPP_API void Exit();
		//Resets the elapsed time counter.
		XNPP_API void ResetElapsedTime();
		//Call this method to initialize the game, begin running the game loop, and start processing events for the game.
		XNPP_API void Run();
		//Run the game through what would happen in a single tick of the game clock; this method is designed for debugging only.
		XNPP_API void RunOneFrame() const;
		//Prevents calls to Draw until the next Update.
		XNPP_API void SuppressDraw() const;
		//Updates the game's clock and calls Update and Draw.
		XNPP_API void Tick();

		//Raised when the game gains focus.
		XNPP_API [[nodiscard]] IEvent<EventArgs>& Activated() const;
		//Raised when the game loses focus.
		XNPP_API [[nodiscard]] IEvent<EventArgs>& Deactivated() const;
		//Raised when the game is being disposed.
		XNPP_API [[nodiscard]] IEvent<EventArgs>& Disposed() const;
		//Raised when the game is exiting.
		XNPP_API [[nodiscard]] IEvent<EventArgs>& Exiting() const;

	protected:
		//A function called when the game starts, before the game loop begins, for the preliminary initialization of the game's logical resources.
		XNPP_API virtual void OnStartRun();
		//Starts the drawing of a frame. This method is followed by calls to Draw and EndDraw.
		XNPP_API virtual bool BeginDraw();
		//Called after all components are initialized but before the first update in the game loop.
		virtual void BeginRun() {};
		//Called when the game determines it is time to draw a frame.
		XNPP_API virtual void Draw(GameTime& gameTime);
		//Ends the drawing of a frame. This method is preceeded by calls to Draw and BeginDraw.
		XNPP_API virtual void EndDraw();
		//Called after the game loop has stopped running before exiting.
		virtual void EndRun() {}
		//Called after the Game and GraphicsDevice are created, but before LoadContent.
		XNPP_API virtual void Initialize();
		//Called when graphics resources need to be loaded.
		virtual void LoadContent() {}
		//Called when graphics resources need to be unloaded. Override this method to unload any game-specific graphics resources.
		virtual void UnloadContent() {}
		//Called when the game has determined that game logic needs to be processed.
		XNPP_API virtual void Update(GameTime& gameTime);

		//Raises the Activated event. Override this method to add code to handle when the game gains focus.
		XNPP_API virtual void OnActivated(void* sender, EventArgs args);
		//Raises the Deactivated event. Override this method to add code to handle when the game loses focus.
		XNPP_API virtual void OnDeactivated(void* sender, EventArgs args);
		//Raises an Exiting event. Override this method to add code to handle when the game is exiting.
		XNPP_API virtual void OnExiting(void* sender, EventArgs args);

	private:
		void RunGame();
		bool ShouldExit();
		void DrawFrame();
		void Paint();
		void EnsureHost();
		void HookDeviceEvents();
		void UnhookDeviceEvents();

		void HostSuspend(void* sender, EventArgs e);
		void HostResume(void* sender, EventArgs e);
		void HostExiting(void* sender, EventArgs e);
		void HostIdle(void* sender, EventArgs e);
		void HostDeactivated(void* sender, EventArgs e);
		void HostActivated(void* sender, EventArgs e);
		void DeviceResetting(void* sender, EventArgs e);
		void DeviceReset(void* sender, EventArgs e);
		void DeviceCreated(void* sender, EventArgs e);
		void GameComponentAdded(void* sender, GameComponentCollectionEventArgs e);
		void GameComponentRemoved(void* sender, GameComponentCollectionEventArgs e);

		void InternalRunGame();

	private:
		struct Implementation;
		std::unique_ptr<Implementation> impl = nullptr;
	};
}