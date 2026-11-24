#pragma once

#include "Xna/Framework/MathTypes.hpp"
#include "Xna/Platform/Platform.hpp"
#include <cstdint>
#include <string>
#include <Xna/CSharp/Event.hpp>

namespace Xna {
	//Defines the display orientation.
	enum class DisplayOrientation
	{
		//The default orientation.
		Default = 0,
		//The display is rotated counterclockwise into a landscape orientation. Width is greater than height.
		LandscapeLeft = 1,
		//The display is rotated clockwise into a landscape orientation. Width is greater than height.
		LandscapeRight = 2,
		//The display is rotated as portrait, where height is greater than width.
		Portrait = 4,
	};

	enum class GameWindowStyle {
		Fullscreen,
		Windowed,
		Borderless,
	};

	class GameWindow {
	public:
		virtual ~GameWindow() = default;

		//
		// Funções que devem ser sobrecarregadas
		//

		//Gets and sets the title of the system window.
		virtual void Title(std::string_view value) { title = value; }
		//Specifies whether to allow the user to resize the game window.
		virtual void AllowUserResizing(bool value) { allowUserResizing = value; }
		//Gets or sets a value indicating whether the mouse cursor should be visible.
		virtual void IsMouseVisible(bool value) { isMouseVisible = value; }
		//Gets or sets whether the window should be minimized
		virtual void IsMinimized(bool value) { isMinimized = value; }

		//
		// Funções virtuais puras que devem ser implementadas pela classe derivada
		//

	public:
		//The screen dimensions of the game window's client rectangle.
		[[nodiscard]] virtual Rectangle ClientBounds() const = 0;
		//Gets the handle to the system window.
		[[nodiscard]] virtual intptr_t Handle() const noexcept = 0;
	protected:
		//Atualização da janela, aplica mudança de estilo, tamanho e outras características).
		virtual void Update() = 0;

	public:
		//Gets the current display orientation, which reflects the physical orientation of the phone in the user's hand.
		[[nodiscard]] DisplayOrientation CurrentOrientation() const noexcept { return displayOrientation; }
		//Gets and sets the title of the system window.
		[[nodiscard]] const std::string& Title() const noexcept { return title; }
		//Specifies whether to allow the user to resize the game window.
		[[nodiscard]] bool AllowUserResizing() const { return allowUserResizing; }
		//Gets or sets a value indicating whether the mouse cursor should be visible.
		[[nodiscard]] constexpr bool IsMouseVisible() const noexcept { return isMouseVisible; }
		//Gets or sets whether the window should be minimized.
		[[nodiscard]] constexpr bool IsMinimized() const { return isMinimized; }		
		[[nodiscard]] constexpr int32_t Width() const noexcept { return width; }
		[[nodiscard]] constexpr int32_t Height() const noexcept { return height; }
		[[nodiscard]] constexpr GameWindowStyle GetStyle() const noexcept { return style; }

		[[nodiscard]] IEvent<EventArgs>& ScreenDeviceNameChanged() noexcept { return screenDeviceNameChanged; }
		[[nodiscard]] IEvent<EventArgs>& ClientSizeChanged() noexcept { return clientSizeChanged; }
		[[nodiscard]] IEvent<EventArgs>& OrientationChanged() noexcept { return orientationChanged; }
		[[nodiscard]] IEvent<EventArgs>& Activated() noexcept { return activated; }
		[[nodiscard]] IEvent<EventArgs>& Deactivated() noexcept { return deactivated; }
		[[nodiscard]] IEvent<EventArgs>& Paint() noexcept { return paint; }
		[[nodiscard]] IEvent<EventArgs>& Suspend() noexcept { return suspend; }
		[[nodiscard]] IEvent<EventArgs>& Resume() noexcept { return resume; }

		virtual void Tick() {}
		virtual void Close() { deactivated.Invoke(this, EventArgs::Empty()); }
		virtual void Create() { activated.Invoke(this, EventArgs::Empty()); }

		virtual void SetSupportedOrientations(DisplayOrientation orientations) { displayOrientation = orientations; }
		constexpr void BeginScreenDeviceChange(bool willBeFullScreen) noexcept { style = willBeFullScreen ? GameWindowStyle::Fullscreen : GameWindowStyle::Windowed; }

		void EndScreenDeviceChange(int32_t clientWidth, int32_t clientHeight) {
			UpdateSize(clientWidth, clientHeight);
			Update();
			ClientSizeChanged();
		}

	protected:
		void UpdateSize(int32_t w, int32_t h) noexcept {
			width = w;
			height = h;
		}

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
