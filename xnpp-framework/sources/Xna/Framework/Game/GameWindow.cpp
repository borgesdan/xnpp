#include "Xna/Framework/Game/GameWindow.hpp"
#include "Xna/Framework/Input/GamePad.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/CSharp/Windows/Forms/SystemInformation.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Platform/Platform.hpp"
#include <optional>

using Xna::CSharp::Screen;

namespace Xna {
	void GameWindow::Title(std::string const& value) {
		Platform::GameWindow_SetTitle(*this, value);
		impl->title = value;
	}			

	std::string GameWindow::ScreenDeviceName() const {
		const auto s = ScreenFromHandle(impl->handle);
		
		return s.has_value() ? s->DeviceName() : "";
	}

	void GameWindow::Create() {
		Platform::GameWindow_Create(*this);
		Activated().Invoke(this, CSharp::EventArgs::Empty);
	}	

	void GameWindow::Close() {
		Deactivated().Invoke(this, CSharp::EventArgs::Empty);
		Platform::GameWindow_Close(*this);
	}		

	void GameWindow::IsMouseVisible(bool value) {
		Platform::GameWindow_SetMouseVisible(*this, value);
		impl->isMouseVisible = value;
	}

	void GameWindow::AllowUserResizing(bool value) {
		if (impl->allowUserResizing == value)
			return;

		Platform::GameWindow_AllowUserResizing(*this, value);

		impl->allowUserResizing = value;
	}

	std::optional<CSharp::Screen> GameWindow::ScreenFromAdapter(GraphicsAdapter const& adapter) {
		const auto screens = CSharp::Screen::AllScreens();

		for (auto& screen : screens) {
			auto deviceNameFromScreen = DeviceNameFromScreen(screen);
			auto deviceNameFromAdapter = adapter.DeviceName();

			if (deviceNameFromScreen == deviceNameFromAdapter)
				return screen;
		}

		return std::nullopt;
	}

	std::string GameWindow::DeviceNameFromScreen(CSharp::Screen const& screen) {
		auto str = screen.DeviceName();
		const auto length = screen.DeviceName().find('\0');

		if (length != -1)
			str = screen.DeviceName().substr(0, length);

		return str;
	}

	std::optional<CSharp::Screen> GameWindow::ScreenFromHandle(intptr_t windowHandle) {
		int num1 = 0;
		std::optional<CSharp::Screen> screen;
		auto rect = Platform::System_WindowRect(windowHandle);
		auto rectangle1 = CSharp::Rectangle::FromLTRB(rect.Left, rect.Top, rect.Right, rect.Bottom);

		auto allScreens = CSharp::Screen::AllScreens();

		for (auto& screen : allScreens) {
			auto rectangle2 = rectangle1;
			rectangle2.Intersect(screen.Bounds());
			int num2 = rectangle2.Width * rectangle2.Height;
			if (num2 > num1)
			{
				num1 = num2;
				screen = screen;
			}
		}

		if (!screen.has_value())
			screen = allScreens[0];

		return screen;
	}
}