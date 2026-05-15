#include "Internal.hpp"
#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/GamePad.hpp"
#include "Xna/Platform/Platform.hpp"
#include <array>
#include <cmath>
#include <SDL3/SDL.h>
#include <stdexcept>

namespace Xna {
	MouseState Platform::Mouse_GetState() {
		float posX, posY;
		auto flags = SDL_GetMouseState(&posX, &posY);

		auto Convert = [](bool pressed)
			{
				return pressed ? ButtonState::Pressed
					: ButtonState::Released;
			};

		MouseState state{};
		state.LeftButton = Convert(flags & SDL_BUTTON_LMASK);
		state.RightButton = Convert(flags & SDL_BUTTON_RMASK);
		state.MiddleButton = Convert(flags & SDL_BUTTON_MMASK);
		state.XButton1 = Convert(flags & SDL_BUTTON_X1MASK);
		state.XButton2 = Convert(flags & SDL_BUTTON_X2MASK);
		state.X = static_cast<int32_t>(posX);
		state.Y = static_cast<int32_t>(posY);
		state.ScroolWheelValue = Cross::Global::MouseWheel;

		return state;
	}

	void Platform::Mouse_SetPosition(int32_t x, int32_t y) {
		if (Cross::Global::CurrentWindowHandle == 0)
			return;

		auto window = reinterpret_cast<SDL_Window*>(Cross::Global::CurrentWindowHandle);
		SDL_WarpMouseInWindow(window, static_cast<float>(x), static_cast<float>(y));
	}

	intptr_t Platform::Mouse_GetWindowHandle() {
		throw std::runtime_error("Platform::Mouse_GetWindowHandle not supported.");
	}

	void Platform::Mouse_SetWindowHandle(intptr_t value) {
		throw std::runtime_error("Platform::Mouse_SetWindowHandle not supported.");
	}

	static void GamepadApplyAxialDeadzone(float& v, float deadzone)
	{
		v = (fabs(v) < deadzone) ? 0.0f : v;
	}

	static void GamepadApplyRadialDeadzone(float& x, float& y, float deadzone)
	{
		const auto magnitudeSquared = x * x + y * y;

		if (magnitudeSquared < deadzone * deadzone)
		{
			x = 0.0f;
			y = 0.0f;
			return;
		}

		const auto magnitude = std::sqrt(x * x + y * y);

		if (magnitude < deadzone)
		{
			x = 0.0f;
			y = 0.0f;
			return;
		}

		const auto newMagnitude = (magnitude - deadzone) / (1.0f - deadzone);
		const auto scale = newMagnitude / magnitude;

		x *= scale;
		y *= scale;

		//Clamp
		if (std::abs(x) > 1.0f) x = (x > 0) ? 1.0f : -1.0f;
		if (std::abs(y) > 1.0f) y = (y > 0) ? 1.0f : -1.0f;
	}

	GamePadState Platform::GamePad_GetState(PlayerIndex playerIndex, GamePadDeadZone deadZone) {
		const auto index = static_cast<int>(playerIndex);

		if (index < 0 || index >= 4)
			return {};

		SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(index);

		if (!pad)
			return {};

		auto ToButtonState = [](SDL_Gamepad* pad, SDL_GamepadButton button)
			{
				return SDL_GetGamepadButton(pad, button)
					? ButtonState::Pressed
					: ButtonState::Released;
			};

		const auto buttons = GamePadButtons(
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_SOUTH),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_EAST),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_WEST),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_NORTH),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_LEFT_STICK),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_RIGHT_STICK),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_BACK),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_START),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_GUIDE));

		const auto dpad = GamePadDPad(
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_DPAD_UP),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_DPAD_DOWN),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_DPAD_LEFT),
			ToButtonState(pad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT));

		const auto isConnected = true;

		constexpr float AXIS_MAX_POS = 32767.0f;
		constexpr float AXIS_MAX_NEG = 32768.0f;

		auto NormalizeAxis = [](Sint16 value)
			{
				return (value < 0)
					? value / AXIS_MAX_POS
					: value / AXIS_MAX_NEG;
			};

		auto lx = NormalizeAxis(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTX));
		auto ly = NormalizeAxis(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTY));
		auto rx = NormalizeAxis(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHTX));
		auto ry = NormalizeAxis(SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHTY));

		constexpr float DEADZONE = 0.15f;

		switch (deadZone) {
		case GamePadDeadZone::Circular:
			GamepadApplyRadialDeadzone(lx, ly, DEADZONE);
			GamepadApplyRadialDeadzone(rx, ry, DEADZONE);
			break;
		case GamePadDeadZone::IndependentAxes:
			GamepadApplyAxialDeadzone(lx, DEADZONE);
			GamepadApplyAxialDeadzone(ly, DEADZONE);
			GamepadApplyAxialDeadzone(rx, DEADZONE);
			GamepadApplyAxialDeadzone(ry, DEADZONE);
			break;
		default:
			break;
		}

		const auto thumbSticks = GamePadThumbSticks(Vector2(lx, ly), Vector2(rx, ry));
		const auto lt = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFT_TRIGGER) / 32767.0f;
		const auto rt = SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER) / 32767.0f;

		const auto triggers = GamePadTriggers(lt, rt);
		const auto padState = GamePadState(thumbSticks, triggers, buttons, dpad, isConnected);
		return padState;
	}

	static GamePadCapabilitiesType ConvertGamepadType(SDL_Gamepad* pad)
	{
		SDL_GamepadType sdlType = SDL_GetGamepadType(pad);

		switch (sdlType)
		{
		case SDL_GAMEPAD_TYPE_STANDARD:
		case SDL_GAMEPAD_TYPE_XBOX360:
		case SDL_GAMEPAD_TYPE_XBOXONE:
		case SDL_GAMEPAD_TYPE_PS3:
		case SDL_GAMEPAD_TYPE_PS4:
		case SDL_GAMEPAD_TYPE_PS5:
		case SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO:
			return GamePadCapabilitiesType::GamePad;

		default:
			break;
		}

		// Fallback mais sofisticado:
		const char* name = SDL_GetGamepadName(pad);
		if (name)
		{
			std::string_view n(name);

			if (n.find("Wheel") != std::string_view::npos)
				return GamePadCapabilitiesType::Wheel;

			if (n.find("Guitar") != std::string_view::npos)
				return GamePadCapabilitiesType::Guitar;

			if (n.find("AlternateGuitar") != std::string_view::npos)
				return GamePadCapabilitiesType::Guitar;

			if (n.find("Drum") != std::string_view::npos)
				return GamePadCapabilitiesType::DrumKit;

			if (n.find("Flight") != std::string_view::npos)
				return GamePadCapabilitiesType::FlightStick;

			if (n.find("BigButton") != std::string_view::npos)
				return GamePadCapabilitiesType::BigButtonPad;
		}

		return GamePadCapabilitiesType::Unknown;
	}

	GamePadCapabilities Platform::GamePad_GetCapabilities(PlayerIndex playerIndex) {
		const auto index = static_cast<int>(playerIndex);

		if (index < 0 || index >= 4)
			return {};

		SDL_Gamepad* pad = SDL_GetGamepadFromPlayerIndex(index);

		if (!pad)
			return {};

		const auto type = ConvertGamepadType(pad);

		return GamePadCapabilities(true, type);
	}

	static Uint16 ConvertRumble(float v)
	{
		v = std::clamp(v, 0.0f, 1.0f);
		return static_cast<Uint16>(v * 65535.0f);
	}

	bool Platform::GamePad_SetVibration(PlayerIndex playerIndex, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger) {
		const auto index = static_cast<int>(playerIndex);

		if (index < 0 || index >= 4)
			return false;

		auto& pad = Cross::Global::Gamepads[index];

		if (!pad.gamepad)
			return false;

		pad.low_frequency_rumble = ConvertRumble(leftMotor);
		pad.high_frequency_rumble = ConvertRumble(rightMotor);
		pad.left_rumble = ConvertRumble(leftTrigger);
		pad.right_rumble = ConvertRumble(rightTrigger);

		return true;
	}

	void Platform::GamePad_Suspend() {
		Cross::Global::SuspendGamepads = true;
	}

	void Platform::GamePad_Resume() {
		Cross::Global::SuspendGamepads = false;
	}

	// Valores VK_ estão no intervalo 0x00–0xFF, 256 entradas.
	static const std::array<SDL_Scancode, 256>& BuildVKToScancodeTable()
	{
		static std::array<SDL_Scancode, 256> table{};
		static bool initialized = false;

		if (!initialized) {
			// Inicializa tudo como UNKNOWN
			for (auto& v : table)
				v = SDL_SCANCODE_UNKNOWN;

			// --- Letras ---
			table['A'] = SDL_SCANCODE_A;
			table['B'] = SDL_SCANCODE_B;
			table['C'] = SDL_SCANCODE_C;
			table['D'] = SDL_SCANCODE_D;
			table['E'] = SDL_SCANCODE_E;
			table['F'] = SDL_SCANCODE_F;
			table['G'] = SDL_SCANCODE_G;
			table['H'] = SDL_SCANCODE_H;
			table['I'] = SDL_SCANCODE_I;
			table['J'] = SDL_SCANCODE_J;
			table['K'] = SDL_SCANCODE_K;
			table['L'] = SDL_SCANCODE_L;
			table['M'] = SDL_SCANCODE_M;
			table['N'] = SDL_SCANCODE_N;
			table['O'] = SDL_SCANCODE_O;
			table['P'] = SDL_SCANCODE_P;
			table['Q'] = SDL_SCANCODE_Q;
			table['R'] = SDL_SCANCODE_R;
			table['S'] = SDL_SCANCODE_S;
			table['T'] = SDL_SCANCODE_T;
			table['U'] = SDL_SCANCODE_U;
			table['V'] = SDL_SCANCODE_V;
			table['W'] = SDL_SCANCODE_W;
			table['X'] = SDL_SCANCODE_X;
			table['Y'] = SDL_SCANCODE_Y;
			table['Z'] = SDL_SCANCODE_Z;

			// --- Números ---
			table['0'] = SDL_SCANCODE_0;
			table['1'] = SDL_SCANCODE_1;
			table['2'] = SDL_SCANCODE_2;
			table['3'] = SDL_SCANCODE_3;
			table['4'] = SDL_SCANCODE_4;
			table['5'] = SDL_SCANCODE_5;
			table['6'] = SDL_SCANCODE_6;
			table['7'] = SDL_SCANCODE_7;
			table['8'] = SDL_SCANCODE_8;
			table['9'] = SDL_SCANCODE_9;

			// --- Teclas de Controle Básicas ---
			table[0x08] = SDL_SCANCODE_BACKSPACE; // VK_BACK
			table[0x09] = SDL_SCANCODE_TAB;       // VK_TAB
			table[0x0D] = SDL_SCANCODE_RETURN;    // VK_RETURN
			table[0x13] = SDL_SCANCODE_PAUSE;     // VK_PAUSE
			table[0x14] = SDL_SCANCODE_CAPSLOCK;  // VK_CAPITAL
			table[0x15] = SDL_SCANCODE_UNKNOWN;   // VK_KANA
			table[0x16] = SDL_SCANCODE_UNKNOWN;   // VK_IME_ON
			table[0x19] = SDL_SCANCODE_UNKNOWN;   // VK_HANJA
			table[0x1A] = SDL_SCANCODE_UNKNOWN;   // VK_IME_OFF
			table[0x1B] = SDL_SCANCODE_ESCAPE;    // VK_ESCAPE
			table[0x1C] = SDL_SCANCODE_UNKNOWN;   // VK_CONVERT
			table[0x1D] = SDL_SCANCODE_UNKNOWN;   // VK_NONCONVERT

			// --- Navegação e Edição ---
			table[0x20] = SDL_SCANCODE_SPACE;       // VK_SPACE
			table[0x21] = SDL_SCANCODE_PAGEUP;      // VK_PRIOR
			table[0x22] = SDL_SCANCODE_PAGEDOWN;    // VK_NEXT
			table[0x23] = SDL_SCANCODE_END;         // VK_END
			table[0x24] = SDL_SCANCODE_HOME;        // VK_HOME
			table[0x25] = SDL_SCANCODE_LEFT;        // VK_LEFT
			table[0x26] = SDL_SCANCODE_UP;          // VK_UP
			table[0x27] = SDL_SCANCODE_RIGHT;       // VK_RIGHT
			table[0x28] = SDL_SCANCODE_DOWN;        // VK_DOWN
			table[0x29] = SDL_SCANCODE_SELECT;      // VK_SELECT
			table[0x2A] = SDL_SCANCODE_UNKNOWN;     // VK_PRINT (Geralmente obsoleto, PrintScreen cobre)
			table[0x2B] = SDL_SCANCODE_EXECUTE;     // VK_EXECUTE
			table[0x2C] = SDL_SCANCODE_PRINTSCREEN; // VK_SNAPSHOT
			table[0x2D] = SDL_SCANCODE_INSERT;      // VK_INSERT
			table[0x2E] = SDL_SCANCODE_DELETE;      // VK_DELETE
			table[0x2F] = SDL_SCANCODE_HELP;        // VK_HELP

			// --- Modificadores (Shift, Ctrl, Alt) ---
			table[0xA0] = SDL_SCANCODE_LSHIFT; // VK_LSHIFT
			table[0xA1] = SDL_SCANCODE_RSHIFT; // VK_RSHIFT
			table[0xA2] = SDL_SCANCODE_LCTRL;  // VK_LCONTROL
			table[0xA3] = SDL_SCANCODE_RCTRL;  // VK_RCONTROL
			table[0xA4] = SDL_SCANCODE_LALT;   // VK_LMENU
			table[0xA5] = SDL_SCANCODE_RALT;   // VK_RMENU

			// --- Teclas do Windows, Menu e Sleep ---
			table[0x5B] = SDL_SCANCODE_LGUI;        // VK_LWIN
			table[0x5C] = SDL_SCANCODE_RGUI;        // VK_RWIN
			table[0x5D] = SDL_SCANCODE_APPLICATION; // VK_APPS
			table[0x5F] = SDL_SCANCODE_SLEEP;       // VK_SLEEP

			// --- Teclado Numérico (Numpad) ---
			table[0x60] = SDL_SCANCODE_KP_0;        // VK_NUMPAD0
			table[0x61] = SDL_SCANCODE_KP_1;        // VK_NUMPAD1
			table[0x62] = SDL_SCANCODE_KP_2;        // VK_NUMPAD2
			table[0x63] = SDL_SCANCODE_KP_3;        // VK_NUMPAD3
			table[0x64] = SDL_SCANCODE_KP_4;        // VK_NUMPAD4
			table[0x65] = SDL_SCANCODE_KP_5;        // VK_NUMPAD5
			table[0x66] = SDL_SCANCODE_KP_6;        // VK_NUMPAD6
			table[0x67] = SDL_SCANCODE_KP_7;        // VK_NUMPAD7
			table[0x68] = SDL_SCANCODE_KP_8;        // VK_NUMPAD8
			table[0x69] = SDL_SCANCODE_KP_9;        // VK_NUMPAD9
			table[0x6A] = SDL_SCANCODE_KP_MULTIPLY; // VK_MULTIPLY
			table[0x6B] = SDL_SCANCODE_KP_PLUS;     // VK_ADD
			table[0x6C] = SDL_SCANCODE_SEPARATOR;   // VK_SEPARATOR
			table[0x6D] = SDL_SCANCODE_KP_MINUS;    // VK_SUBTRACT
			table[0x6E] = SDL_SCANCODE_KP_PERIOD;   // VK_DECIMAL
			table[0x6F] = SDL_SCANCODE_KP_DIVIDE;   // VK_DIVIDE
			table[0x90] = SDL_SCANCODE_NUMLOCKCLEAR;// VK_NUMLOCK
			table[0x91] = SDL_SCANCODE_SCROLLLOCK;  // VK_SCROLL

			// --- Teclas de Função F1 ao F24 ---
			table[0x70] = SDL_SCANCODE_F1;  // VK_F1
			table[0x71] = SDL_SCANCODE_F2;  // VK_F2
			table[0x72] = SDL_SCANCODE_F3;  // VK_F3
			table[0x73] = SDL_SCANCODE_F4;  // VK_F4
			table[0x74] = SDL_SCANCODE_F5;  // VK_F5
			table[0x75] = SDL_SCANCODE_F6;  // VK_F6
			table[0x76] = SDL_SCANCODE_F7;  // VK_F7
			table[0x77] = SDL_SCANCODE_F8;  // VK_F8
			table[0x78] = SDL_SCANCODE_F9;  // VK_F9
			table[0x79] = SDL_SCANCODE_F10; // VK_F10
			table[0x7A] = SDL_SCANCODE_F11; // VK_F11
			table[0x7B] = SDL_SCANCODE_F12; // VK_F12
			table[0x7C] = SDL_SCANCODE_F13; // VK_F13
			table[0x7D] = SDL_SCANCODE_F14; // VK_F14
			table[0x7E] = SDL_SCANCODE_F15; // VK_F15
			table[0x7F] = SDL_SCANCODE_F16; // VK_F16
			table[0x80] = SDL_SCANCODE_F17; // VK_F17
			table[0x81] = SDL_SCANCODE_F18; // VK_F18
			table[0x82] = SDL_SCANCODE_F19; // VK_F19
			table[0x83] = SDL_SCANCODE_F20; // VK_F20
			table[0x84] = SDL_SCANCODE_F21; // VK_F21
			table[0x85] = SDL_SCANCODE_F22; // VK_F22
			table[0x86] = SDL_SCANCODE_F23; // VK_F23
			table[0x87] = SDL_SCANCODE_F24; // VK_F24

			// --- Controles de Navegador e Mídia ---
			table[0xA6] = SDL_SCANCODE_AC_BACK;      // VK_BROWSER_BACK
			table[0xA7] = SDL_SCANCODE_AC_FORWARD;   // VK_BROWSER_FORWARD
			table[0xA8] = SDL_SCANCODE_AC_REFRESH;   // VK_BROWSER_REFRESH
			table[0xA9] = SDL_SCANCODE_AC_STOP;      // VK_BROWSER_STOP
			table[0xAA] = SDL_SCANCODE_AC_SEARCH;    // VK_BROWSER_SEARCH
			table[0xAB] = SDL_SCANCODE_AC_BOOKMARKS; // VK_BROWSER_FAVORITES
			table[0xAC] = SDL_SCANCODE_AC_HOME;      // VK_BROWSER_HOME
			table[0xAD] = SDL_SCANCODE_UNKNOWN;    // VK_VOLUME_MUTE
			table[0xAE] = SDL_SCANCODE_VOLUMEDOWN;   // VK_VOLUME_DOWN
			table[0xAF] = SDL_SCANCODE_VOLUMEUP;     // VK_VOLUME_UP
			table[0xB0] = SDL_SCANCODE_UNKNOWN;    // VK_MEDIA_NEXT_TRACK
			table[0xB1] = SDL_SCANCODE_UNKNOWN;    // VK_MEDIA_PREV_TRACK
			table[0xB2] = SDL_SCANCODE_UNKNOWN;    // VK_MEDIA_STOP
			table[0xB3] = SDL_SCANCODE_UNKNOWN;    // VK_MEDIA_PLAY_PAUSE
			table[0xB4] = SDL_SCANCODE_UNKNOWN;         // VK_LAUNCH_MAIL
			table[0xB5] = SDL_SCANCODE_UNKNOWN;  // VK_LAUNCH_MEDIA_SELECT
			table[0xB6] = SDL_SCANCODE_UNKNOWN;         // VK_LAUNCH_APP1
			table[0xB7] = SDL_SCANCODE_UNKNOWN;         // VK_LAUNCH_APP2

			// --- Teclas OEM (Mapeadas pela posição física no teclado US) ---
			table[0xBA] = SDL_SCANCODE_SEMICOLON;    // VK_OEM_1 (;: US)
			table[0xBB] = SDL_SCANCODE_EQUALS;       // VK_OEM_PLUS (=+)
			table[0xBC] = SDL_SCANCODE_COMMA;        // VK_OEM_COMMA (,<)
			table[0xBD] = SDL_SCANCODE_MINUS;        // VK_OEM_MINUS (-_)
			table[0xBE] = SDL_SCANCODE_PERIOD;       // VK_OEM_PERIOD (.>)
			table[0xBF] = SDL_SCANCODE_SLASH;        // VK_OEM_2 (/? US)
			table[0xC0] = SDL_SCANCODE_GRAVE;        // VK_OEM_3 (`~ US)
			table[0xDB] = SDL_SCANCODE_LEFTBRACKET;  // VK_OEM_4 ([{ US)
			table[0xDC] = SDL_SCANCODE_BACKSLASH;    // VK_OEM_5 (\| US)
			table[0xDD] = SDL_SCANCODE_RIGHTBRACKET; // VK_OEM_6 (]} US)
			table[0xDE] = SDL_SCANCODE_APOSTROPHE;   // VK_OEM_7 ('" US)
			table[0xDF] = SDL_SCANCODE_UNKNOWN;      // VK_OEM_8
			table[0xE2] = SDL_SCANCODE_NONUSBACKSLASH;// VK_OEM_102 (Tecla Extra entre LShift e Z no ISO)

			// --- Teclas Legadas e Especiais Específicas do Hardware ---
			table[0xE5] = SDL_SCANCODE_UNKNOWN;      // VK_PROCESSKEY
			table[0xF2] = SDL_SCANCODE_UNKNOWN;      // VK_OEM_COPY
			table[0xF3] = SDL_SCANCODE_UNKNOWN;      // VK_OEM_AUTO
			table[0xF4] = SDL_SCANCODE_UNKNOWN;      // VK_OEM_ENLW
			table[0xF6] = SDL_SCANCODE_UNKNOWN;      // VK_ATTN
			table[0xF7] = SDL_SCANCODE_CRSEL;        // VK_CRSEL
			table[0xF8] = SDL_SCANCODE_EXSEL;        // VK_EXSEL
			table[0xF9] = SDL_SCANCODE_UNKNOWN;      // VK_EREOF
			table[0xFA] = SDL_SCANCODE_UNKNOWN;      // VK_PLAY
			table[0xFB] = SDL_SCANCODE_UNKNOWN;      // VK_ZOOM
			table[0xFD] = SDL_SCANCODE_UNKNOWN;      // VK_PA1
			table[0xFE] = SDL_SCANCODE_CLEAR;        // VK_OEM_CLEAR

			// FLAG para evitar recalcular a tabela inteira nas próximas chamadas.
			initialized = true;
		}

		return table;
	}

	static SDL_Scancode WinVKToSDLScancode(Keys key)
	{
		auto& VKToScancodeTable = BuildVKToScancodeTable();

		return VKToScancodeTable[
			static_cast<unsigned int>(key) & 0xFF
		];
	}

	//Helper para setar bit por índice VK
	// [1] KeyboardState bit-packed (256 bits)
	// [2] Indexado diretamente por valor VK_
	// [3] Replica o layout DirectXTK
	static void SetKeyBit(KeyboardState& state, unsigned int vk)
	{
		auto ptr = reinterpret_cast<uint32_t*>(&state);

		const unsigned int index = vk >> 5;      // divide por 32
		const unsigned int bit = vk & 0x1f;    // mod 32

		ptr[index] |= (1u << bit);
	}

	KeyboardState Platform::Keyboard_GetState() {
		int numKeys = 0;
		const bool* sdlState = SDL_GetKeyboardState(&numKeys);

		// Inicializa toda a union/struct com zeros automaticamente
		KeyboardState state{};

		for (unsigned int vk = 0; vk <= 0xFE; ++vk) {
			SDL_Scancode sc = WinVKToSDLScancode(static_cast<Keys>(vk));

			if (sc != SDL_SCANCODE_UNKNOWN && sc < numKeys && sdlState[sc]) {
				SetKeyBit(state, vk);
			}
		}

		return state;
	}
}