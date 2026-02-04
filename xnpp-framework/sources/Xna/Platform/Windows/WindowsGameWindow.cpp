#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS

#include "Xna/Platform/Windows/WindowsPlatform.hpp"
#include "Xna/Platform/Windows/WindowsPlatformHelpers.hpp"
#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/GamePad.hpp"
#include "Xna/Framework/Game/GameWindow.hpp"

namespace Xna {
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);		

	void Platform::GameWindow_Create(GameWindow const& gw) {
		auto& impl = gw.impl;
		auto& implP = gw.impl->platformImpl;

		WNDCLASSEX wndClass{};
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WinProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = implP.instance;
		wndClass.hIcon = implP.icon;
		wndClass.hCursor = implP.cursor;
		wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(implP.color);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "XnaGameWindow";
		wndClass.hIconSm = implP.icon;

		if (!RegisterClassEx(&wndClass))
			throw CSharp::InvalidOperationException("RegisterClassEx() failed.");

		implP.handle = CreateWindowEx(
			NULL,
			"XnaGameWindow",
			impl->title.c_str(),
			implP.style,
			implP.positionX,
			implP.positionY,
			implP.width,
			implP.height,
			NULL,
			NULL,
			implP.instance,
			NULL);

		if (!implP.handle)
			throw CSharp::InvalidOperationException("CreateWindowEx() failed.");

		impl->handle = reinterpret_cast<intptr_t>(implP.handle);
		
		Platform::GameWindow_Update(gw);		
	}

	void Platform::GameWindow_Update(GameWindow const& gw) {		
		auto& impl = gw.impl->platformImpl;

		if (impl.handle == 0)
			return;

		if (impl.style == static_cast<int>(PlatformImpl::GameWindowImpl::GameWindowMode::Windowed)) {
			RECT winRect = { 0, 0, impl.width, impl.height };

			AdjustWindowRectEx(&winRect,
				GetWindowStyle(impl.handle),
				GetMenu(impl.handle) != NULL,
				GetWindowExStyle(impl.handle));

			impl.positionX = GetSystemMetrics(SM_CXSCREEN) / 2 - (winRect.right - winRect.left) / 2;
			impl.positionY = GetSystemMetrics(SM_CYSCREEN) / 2 - (winRect.bottom - winRect.top) / 2;

			MoveWindow(
				impl.handle,
				impl.positionX,
				impl.positionY,
				winRect.right - winRect.left,
				winRect.bottom - winRect.top,
				TRUE);
		}
	}

	Rectangle Platform::GameWindow_ClientBounds(GameWindow const& gw) {
		auto& impl = gw.impl->platformImpl;

		return {
			impl.positionX,
			impl.positionY,
			impl.width,
			impl.height
		};
	}

	void Platform::GameWindow_SetTitle(GameWindow const& gw, std::string const& title) {
		auto& impl = gw.impl->platformImpl;

		if (impl.handle != NULL)
			SetWindowText(impl.handle, LPCSTR(title.c_str()));
	}

	void Platform::GameWindow_Close(GameWindow const& gw) {
		CloseWindow(gw.impl->platformImpl.handle);
	}

	bool Platform::GameWindow_WindowIsMinimized(GameWindow const& gw) {
		return static_cast<bool>(IsIconic(gw.impl->platformImpl.handle));
	}

	void Platform::GameWindow_MinimizeWindow(GameWindow const& gw, bool value) {
		//https://learn.microsoft.com/pt-br/windows/win32/api/winuser/nf-winuser-showwindow

		if (value)
			ShowWindow(gw.impl->platformImpl.handle, SW_MINIMIZE);
		else
			ShowWindow(gw.impl->platformImpl.handle, SW_SHOW);			
	}

	void Platform::GameWindow_SetMouseVisible(GameWindow const& gw, bool value) {
		if (value)
			while (ShowCursor(TRUE) <= 0);
		else
			while (ShowCursor(FALSE) >= 0);
	}

	void Platform::GameWindow_AllowUserResizing(GameWindow const& gw, bool value) {
		auto& impl = gw.impl->platformImpl;

		auto style = GetWindowLongPtr(impl.handle, GWL_STYLE);

		if (value)
			style |= WS_SIZEBOX;
		else
			style &= ~WS_SIZEBOX;

		SetWindowLongPtr(impl.handle, GWL_STYLE, style);
	}

	void Platform::GameWindow_BeginScreenDeviceChange(GameWindow const& gw, bool willBeFullScreen) {
		gw.impl->platformImpl.style = static_cast<DWORD>(willBeFullScreen ? PlatformImpl::GameWindowImpl::GameWindowMode::Fullscreen : PlatformImpl::GameWindowImpl::GameWindowMode::Windowed);
	}

	void Platform::GameWindow_EndScreenDeviceChange(GameWindow const& gw, std::string const& screenDeviceName, int32_t clientWidth, int32_t clientHeight) {
		gw.impl->platformImpl.width = clientWidth;
		gw.impl->platformImpl.height = clientHeight;
		GameWindow_Update(gw);
	}

	LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		auto imsg = Platform::InputProcessMessage();
		imsg.msg = msg;
		imsg.wParam = wParam;
		imsg.lParam = lParam;

		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:			
			Platform::Keyboard_ProcessMessage(imsg);
			Platform::Mouse_ProcessMessage(imsg);
			break;
		case WM_SYSKEYDOWN:
			if (!(wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)) {
				Platform::Keyboard_ProcessMessage(imsg);
			}
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Platform::Keyboard_ProcessMessage(imsg);
			break;
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Platform::Mouse_ProcessMessage(imsg);
			break;
		case WM_KILLFOCUS:
			Platform::GamePad_Suspend();
			break;
		case WM_SETFOCUS:
			Platform::GamePad_Resume();
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

#endif