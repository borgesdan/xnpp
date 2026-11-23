#pragma once

#include <string>
#include <memory>
#include <cstdint>
#include <Xna/CSharp/Windows/Forms/Screen.hpp>
#include <Xna/CSharp/Event.hpp>
#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/Framework/MathTypes.hpp"
#include "Xna/Framework/Shared.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	enum class GameWindowStyle {
		Fullscreen,
		Windowed,
		Borderless,
	};

	struct Screen {	
		intptr_t _hmonitor{ 0 };
		Rectangle _bounds{};
		Rectangle _workingArea{};
		bool _primary{ false };
		std::string _deviceName;
		int32_t _bitDepth{ 0 };
		int32_t _currentDesktopChangedCount{ -1 };
	};

	class IGameWindow {
	public:
		IGameWindow() = default;
		virtual ~IGameWindow() = default;

		//Gets and sets the title of the system window.
		virtual void Title(std::string_view value) { title = value; }
		//The screen dimensions of the game window's client rectangle.
		virtual Rectangle ClientBounds() const = 0;
		//Gets the handle to the system window.
		virtual intptr_t Handle() const noexcept = 0;
		//Specifies whether to allow the user to resize the game window.
		virtual void AllowUserResizing(bool value) { allowUserResizing = value; }
		//Gets or sets a value indicating whether the mouse cursor should be visible.
		virtual void IsMouseVisible(bool value) { isMouseVisible = value; }
		//Define que a janela deve ser minimizada
		virtual void IsMinimized(bool value) { isMinimized = value; }


		//Gets the current display orientation, which reflects the physical orientation of the phone in the user's hand.
		DisplayOrientation CurrentOrientation() const noexcept { return displayOrientation; }
		//Gets and sets the title of the system window.
		const std::string& Title() const noexcept { return title; }
		//Specifies whether to allow the user to resize the game window.
		bool AllowUserResizing() const { return allowUserResizing; }		

		IEvent<EventArgs>& ScreenDeviceNameChanged() { return screenDeviceNameChanged; }
		IEvent<EventArgs>& ClientSizeChanged() { return clientSizeChanged; }
		IEvent<EventArgs>& OrientationChanged() { return orientationChanged; }
		IEvent<EventArgs>& Activated() { return activated; }
		IEvent<EventArgs>& Deactivated() { return deactivated; }
		IEvent<EventArgs>& Paint() { return paint; }
		IEvent<EventArgs>& Suspend() { return suspend; }
		IEvent<EventArgs>& Resume() { return resume; }

		virtual void Tick() {}

		virtual void Close() { deactivated.Invoke(this, EventArgs::Empty()); }
		virtual void Create() { activated.Invoke(this, EventArgs::Empty()); }

		constexpr int32_t Width() const noexcept { return width; }
		constexpr int32_t Height() const noexcept { return height; }
		constexpr GameWindowStyle GetStyle() const noexcept { return style; }
		//Gets or sets a value indicating whether the mouse cursor should be visible.
		constexpr bool IsMouseVisible() const noexcept { return isMouseVisible; }
		constexpr bool IsMinimized() const { return isMinimized; }

		virtual void SetSupportedOrientations(DisplayOrientation orientations) { displayOrientation = orientations; }
		constexpr void BeginScreenDeviceChange(bool willBeFullScreen) { style = willBeFullScreen ? GameWindowStyle::Fullscreen : GameWindowStyle::Windowed; }

		void EndScreenDeviceChange(std::string_view screenDeviceName) {
			const auto clientBounds = ClientBounds();
			EndScreenDeviceChange(screenDeviceName, clientBounds.Width, clientBounds.Height);
		}

		void EndScreenDeviceChange(std::string_view screenDeviceName, int32_t clientWidth, int32_t clientHeight) {
			height = clientHeight;
			width = clientWidth;

			Update();
			ClientSizeChanged();
		}		

	protected:
		virtual void Update() = 0;

	private:
		bool allowUserResizing{ false };
		bool isMouseVisible{ false };
		bool isMinimized{ false };

		DisplayOrientation displayOrientation{ DisplayOrientation::Default };

		int32_t width{ 800 };
		int32_t height{ 600 };

		GameWindowStyle style{ GameWindowStyle::Windowed };

		std::string title{ "Super XNPP Game" };

		Event<EventArgs> screenDeviceNameChanged;
		Event<EventArgs> clientSizeChanged;
		Event<EventArgs> orientationChanged;
		Event<EventArgs> activated;
		Event<EventArgs> deactivated;
		Event<EventArgs> paint;
		Event<EventArgs> suspend;
		Event<EventArgs> resume;
	};
}
