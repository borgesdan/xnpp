#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Input/KeyboardState.hpp"
#include <SDL3/SDL.h>
#include <array>

namespace Xna {
	//Valores VK_ estão no intervalo 0x00–0xFF, 256 entradas.
	static const std::array<SDL_Scancode, 256>& BuildVKToScancodeTable()
	{
		static std::array<SDL_Scancode, 256> table{};
		static bool initialized = false;

		if (!initialized) {
			// Inicializa tudo como UNKNOWN
			for (auto& v : table)
				v = SDL_SCANCODE_UNKNOWN;

			// Letras
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
			
			table[0x08] = SDL_SCANCODE_BACKSPACE; //VK_BACK
			table[0x09] = SDL_SCANCODE_TAB; //VK_TAB
			table[0x0D] = SDL_SCANCODE_RETURN; //VK_RETURN
			table[0x1B] = SDL_SCANCODE_ESCAPE; //VK_ESCAPE
			table[0x20] = SDL_SCANCODE_SPACE; //VK_SPACE

			table[0x25] = SDL_SCANCODE_LEFT; //VK_LEFT
			table[0x27] = SDL_SCANCODE_RIGHT; //VK_RIGHT
			table[0x26] = SDL_SCANCODE_UP; //VK_UP
			table[0x28] = SDL_SCANCODE_DOWN; //VK_DOWN

			table[0xA0] = SDL_SCANCODE_LSHIFT; //VK_LSHIFT
			table[0xA1] = SDL_SCANCODE_RSHIFT; //VK_RSHIFT
			table[0xA2] = SDL_SCANCODE_LCTRL; //VK_LCONTROL
			table[0xA3] = SDL_SCANCODE_RCTRL; //VK_RCONTROL
			table[0xA4] = SDL_SCANCODE_LALT; //VK_LMENU
			table[0xA5] = SDL_SCANCODE_RALT; //VK_RMENU

			table[0x70] = SDL_SCANCODE_F1; //VK_F1
			table[0x71] = SDL_SCANCODE_F2; //VK_F2
			table[0x72] = SDL_SCANCODE_F3; //VK_F3
			table[0x73] = SDL_SCANCODE_F4; //VK_F4
			table[0x74] = SDL_SCANCODE_F5; //VK_F5
			table[0x75] = SDL_SCANCODE_F6; //VK_F6
			table[0x76] = SDL_SCANCODE_F7; //VK_F7
			table[0x77] = SDL_SCANCODE_F8; //VK_F8
			table[0x78] = SDL_SCANCODE_F9; //VK_F9
			table[0x79] = SDL_SCANCODE_F10; //VK_F10
			table[0x7A] = SDL_SCANCODE_F11; //VK_F11
			table[0x7B] = SDL_SCANCODE_F12; //VK_F12
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

		KeyboardState state{};
		auto ptr = reinterpret_cast<uint32_t*>(&state);

		for (int i = 0; i < 8; ++i)
			ptr[i] = 0;

		for (unsigned int vk = 0; vk <= 0xFE; ++vk)
		{
			SDL_Scancode sc = WinVKToSDLScancode(static_cast<Keys>(vk));

			if (sc != SDL_SCANCODE_UNKNOWN &&
				sc < numKeys &&
				sdlState[sc])
			{
				SetKeyBit(state, vk);
			}
		}

		return state;
	}

	void Platform::Keyboard_ProcessMessage(InputProcessMessage const& msg) {
		//WindowsPlatform::GetKeyboard().ProcessMessage(msg.msg, msg.wParam, msg.lParam);
	}
}