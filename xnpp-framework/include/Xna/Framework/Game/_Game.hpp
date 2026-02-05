#ifndef XNA_FRAMEWORK_GAME__GAME_HPP
#define XNA_FRAMEWORK_GAME__GAME_HPP

#include <memory>
#include "Xna/CSharp/TimeSpan.hpp"
#include "Xna/CSharp/Event.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	struct GameComponentCollectionEventArgs;
	class GameComponentCollection;
	class ContentManager;
	class GraphicsDevice;
	class LaunchParameters;
	class GameServiceContainer;
	class GameWindow;
	class GameTime;

	//Provides basic graphics device initialization, game logic, and rendering code.
	class Game {
	public:
		//Initializes a new instance of this class, which provides basic graphics device initialization, game logic, rendering code, and a game loop.
		XNPP_API Game();

		//Gets the collection of GameComponents owned by the game.
		inline GameComponentCollection Components() const;
		//Gets or sets the current ContentManager.
		XNPP_API ContentManager Content() const;
		//Gets or sets the current ContentManager.
		void Content(ContentManager const& value);
		//Gets the current GraphicsDevice.
		XNPP_API Xna::GraphicsDevice GraphicsDevice() const;
		//Gets or sets the time to sleep when the game is inactive.
		XNPP_API CSharp::TimeSpan InactiveSleepTime() const;
		//Gets or sets the time to sleep when the game is inactive.
		XNPP_API void InactiveSleepTime(CSharp::TimeSpan const& value);
		//Indicates whether the game is currently the active application.
		XNPP_API bool IsActive() const;
		//Gets or sets a value indicating whether to use fixed time steps.
		XNPP_API bool IsFixedTimeStep() const;
		//Gets or sets a value indicating whether to use fixed time steps.
		XNPP_API void IsFixedTimeStep(bool value);
		//Gets or sets a value indicating whether the mouse cursor should be visible.
		XNPP_API bool IsMouseVisible() const;
		//Gets or sets a value indicating whether the mouse cursor should be visible.
		XNPP_API void IsMouseVisible(bool value);
		//Gets the start up parameters in LaunchParameters.
		XNPP_API Xna::LaunchParameters LaunchParameters() const;
		//Gets the start up parameters in LaunchParameters.
		XNPP_API void LaunchParameters(Xna::LaunchParameters const& value);
		//Gets the GameServiceContainer holding all the service providers attached to the Game.
		XNPP_API GameServiceContainer Services() const;
		//Gets or sets the target time between calls to Update when IsFixedTimeStep is true.
		XNPP_API CSharp::TimeSpan TargetElapsedTime() const;
		//Gets or sets the target time between calls to Update when IsFixedTimeStep is true.
		XNPP_API void TargetElapsedTime(CSharp::TimeSpan const& value);
		//Gets the underlying operating system window.
		XNPP_API std::optional<GameWindow> Window() const;

		//Exits the game.
		XNPP_API void Exit();
		//Resets the elapsed time counter.
		XNPP_API void ResetElapsedTime();
		//Call this method to initialize the game, begin running the game loop, and start processing events for the game.
		XNPP_API void Run();
		//Run the game through what would happen in a single tick of the game clock; this method is designed for debugging only.
		XNPP_API void RunOneFrame();
		//Prevents calls to Draw until the next Update.
		XNPP_API void SuppressDraw();
		//Updates the game's clock and calls Update and Draw.
		XNPP_API void Tick();

		//Raised when the game gains focus.
		XNPP_API CSharp::Event<CSharp::EventArgs> Activated() const;
		//Raised when the game loses focus.
		XNPP_API CSharp::Event<CSharp::EventArgs> Deactivated() const;
		//Raised when the game is being disposed.
		XNPP_API CSharp::Event<CSharp::EventArgs> Disposed() const;
		//Raised when the game is exiting.
		XNPP_API CSharp::Event<CSharp::EventArgs> Exiting() const;

	protected:
		//Starts the drawing of a frame. This method is followed by calls to Draw and EndDraw.
		XNPP_API virtual bool BeginDraw();
		//Called after all components are initialized but before the first update in the game loop.
		XNPP_API virtual void BeginRun(){}
		//Called when the game determines it is time to draw a frame.
		XNPP_API virtual void Draw(GameTime& gameTime);
		//Ends the drawing of a frame. This method is preceeded by calls to Draw and BeginDraw.
		XNPP_API virtual void EndDraw();
		//Called after the game loop has stopped running before exiting.
		XNPP_API virtual void EndRun(){}
		//Called after the Game and GraphicsDevice are created, but before LoadContent.
		XNPP_API virtual void Initialize();
		//Called when graphics resources need to be loaded.
		XNPP_API virtual void LoadContent(){}
		//Called when graphics resources need to be unloaded. Override this method to unload any game-specific graphics resources.
		XNPP_API virtual void UnloadContent(){}
		//Called when the game has determined that game logic needs to be processed.
		XNPP_API virtual void Update(GameTime& gameTime);

		//Raises the Activated event. Override this method to add code to handle when the game gains focus.
		XNPP_API virtual void OnActivated(void* sender, CSharp::EventArgs args);
		//Raises the Deactivated event. Override this method to add code to handle when the game loses focus.
		XNPP_API virtual void OnDeactivated(void* sender, CSharp::EventArgs args);
		//Raises an Exiting event. Override this method to add code to handle when the game is exiting.
		XNPP_API virtual void OnExiting(void* sender, CSharp::EventArgs args);

		inline Game(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(Game const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		void RunGame(bool useBlockingRun);
		inline void StartGameLoop() { RunGame(false); }
		bool ShouldExit();
		void DrawFrame();
		void Paint();
		void EnsureHost();
		void HookDeviceEvents();
		void UnhookDeviceEvents();

		void HostSuspend(void* sender, CSharp::EventArgs const& e);
		void HostResume(void* sender, CSharp::EventArgs const& e);
		void HostExiting(void* sender, CSharp::EventArgs const& e);
		void HostIdle(void* sender, CSharp::EventArgs const& e);
		void HostDeactivated(void* sender, CSharp::EventArgs const& e);
		void HostActivated(void* sender, CSharp::EventArgs const& e);

		void DeviceResetting(void* sender, CSharp::EventArgs const& e);
		void DeviceReset(void* sender, CSharp::EventArgs const& e);
		void DeviceCreated(void* sender, CSharp::EventArgs const& e);	
		bool IsActiveIgnoringGuide() const { return true; } //TODO

		void GameComponentAdded(void* sender, GameComponentCollectionEventArgs e);
		void GameComponentRemoved(void* sender, GameComponentCollectionEventArgs e);

	public:
		struct Implementation;
		std::shared_ptr<Implementation> impl = nullptr;	

		friend class GraphicsDeviceManager;
	};
}

#endif