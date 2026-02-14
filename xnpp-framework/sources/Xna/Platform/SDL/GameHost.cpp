#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Game/GameHost.hpp"
#include <SDL3/SDL.h>

namespace Xna {
	void Platform::GameHost_Tick(GameHost& gh) {
        SDL_Event event;
        bool running = true;

        while (running) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    //WM_DESTROY
                case SDL_EVENT_QUIT:
                    running = false;
                    break;

                    //WM_ACTIVATE e WM_SETFOCUS / WM_KILLFOCUS
                case SDL_EVENT_WINDOW_FOCUS_GAINED:
                    Platform::GamePad_Resume();
                    break;
                case SDL_EVENT_WINDOW_FOCUS_LOST:
                    Platform::GamePad_Suspend();
                    break;

                    // WM_KEYDOWN, WM_KEYUP e WM_SYSKEYDOWN
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                    Platform::Keyboard_ProcessMessage(event);
                    break;

                    // Substitui WM_MOUSEMOVE, WM_LBUTTONDOWN, etc.
                case SDL_EVENT_MOUSE_MOTION:
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_UP:
                case SDL_EVENT_MOUSE_WHEEL:                    
                    Platform::Mouse_ProcessMessage(event);
                    break;
                }
            }
            
            if (gh.impl->exitRequested || !running) 
                gh.impl->gameWindow.Close();
            else 
                // Executa a lógica do jogo e o Render do DirectX
                gh.RunOneFrame();
        }
	}
}
