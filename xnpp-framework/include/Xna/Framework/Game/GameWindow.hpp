#ifndef XNA_FRAMEWORK_GAME_GAMEWINDOW_HPP
#define XNA_FRAMEWORK_GAME_GAMEWINDOW_HPP

#include <string>
#include <memory>
#include "Xna/CSharp/Windows/Forms/Screen.hpp"
#include "Xna/CSharp/Event.hpp"
#include "Xna/CSharp/Windows/Forms/Screen.hpp"
#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/Framework/Rectangle.hpp"
#include "Xna/Framework/Shared.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	class GameWindow {
	public:
		//Gets and sets the title of the system window.
		XNPP_API void Title(std::string const& value);
		//The screen dimensions of the game window's client rectangle.
		inline Rectangle ClientBounds() const { return Platform::GameWindow_ClientBounds(*this); }
		//Gets the current display orientation, which reflects the physical orientation of the phone in the user's hand.
		inline DisplayOrientation CurrentOrientation() const { return impl->displayOrientation; }
		//Gets the handle to the system window.
		inline intptr_t Handle() const { return impl->handle; }
		//Gets the device name of the screen the window is currently in.
		XNPP_API std::string ScreenDeviceName() const;
		//Gets and sets the title of the system window.
		inline std::string Title() const { return impl->title; }

		//Specifies whether to allow the user to resize the game window.
		bool AllowUserResizing() const { return impl->allowUserResizing; }
		//Specifies whether to allow the user to resize the game window.
		XNPP_API void AllowUserResizing(bool value);

		CSharp::Event<CSharp::EventArgs> ScreenDeviceNameChanged() { return impl->ScreenDeviceNameChanged; }
		CSharp::Event<CSharp::EventArgs> ClientSizeChanged() { return impl->ClientSizeChanged; }
		CSharp::Event<CSharp::EventArgs> OrientationChanged() { return impl->OrientationChanged; }

		inline GameWindow(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(GameWindow const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

		enum class Style {
			Fullscreen,
			Windowed,
			Borderless,
		};

	protected:
		virtual void SetSupportedOrientations(DisplayOrientation orientations) {};

	private:
		bool IsMouseVisible() const { return impl->isMouseVisible; }
		void IsMouseVisible(bool value);
		bool (IsMinimized)() const { return Platform::GameWindow_WindowIsMinimized(*this); }
		void (IsMinimized)(bool value) { Platform::GameWindow_MinimizeWindow(*this, value); }

		inline void BeginScreenDeviceChange(bool willBeFullScreen) { 
			impl->style = willBeFullScreen ? Style::Fullscreen : Style::Windowed;
		}

		inline void EndScreenDeviceChange(std::string const& screenDeviceName) {
			const auto clientBounds = ClientBounds();
			EndScreenDeviceChange(screenDeviceName, clientBounds.Width, clientBounds.Height);
		}

		inline void EndScreenDeviceChange(std::string const& screenDeviceName, int32_t clientWidth, int32_t clientHeight) { 
			impl->height = clientHeight;
			impl->width = clientWidth;
			Platform::GameWindow_Update(*this);
		}

		void Close();
		void Tick() {}
		void Create();

		static std::optional<CSharp::Screen> ScreenFromAdapter(GraphicsAdapter const& adapter);
		static std::optional<CSharp::Screen> ScreenFromHandle(intptr_t windowHandle);

		static std::string DeviceNameFromScreen(CSharp::Screen const& screen);

		CSharp::Event<CSharp::EventArgs> Activated() { return impl->Activated; }
		CSharp::Event<CSharp::EventArgs> Deactivated() { return impl->Deactivated; }
		CSharp::Event<CSharp::EventArgs> Paint() { return impl->Paint; }
		CSharp::Event<CSharp::EventArgs> Suspend() { return impl->Suspend; }
		CSharp::Event<CSharp::EventArgs> Resume() { return impl->Resume; }

		inline GameWindow() { impl = std::make_shared<GameWindow::Implementation>(); }

	private:
		struct Implementation {
			std::string title{ "XN65 Game" };
			std::string screenDeviceName;
			intptr_t handle{ 0 };
			DisplayOrientation displayOrientation{ DisplayOrientation::Default };
			bool allowUserResizing{ false };
			bool isMouseVisible{ false };
			int32_t width{ 800 };
			int32_t height{ 480 };
			Style style{ Style::Windowed };

			CSharp::Event<CSharp::EventArgs> ScreenDeviceNameChanged;
			CSharp::Event<CSharp::EventArgs> ClientSizeChanged;
			CSharp::Event<CSharp::EventArgs> OrientationChanged;
			CSharp::Event<CSharp::EventArgs> Activated;
			CSharp::Event<CSharp::EventArgs> Deactivated;
			CSharp::Event<CSharp::EventArgs> Paint;
			CSharp::Event<CSharp::EventArgs> Suspend;
			CSharp::Event<CSharp::EventArgs> Resume;

			PlatformImpl::GameWindowImpl platformImpl;
		};
		std::shared_ptr<Implementation> impl = nullptr;	

		friend class Game;
		friend class GameHost;
		friend class GraphicsDeviceManager;
		friend struct Platform;
	};
}
#endif