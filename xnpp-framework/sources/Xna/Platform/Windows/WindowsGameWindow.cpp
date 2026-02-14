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