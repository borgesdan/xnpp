#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Game/GameHost.hpp"
#include "InternalSdl.hpp"
#include <SDL3/SDL.h>

namespace Xna {
    static inline int g_MouseWheel4 = 0;
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

                    // Substitui WM_MOUSEMOVE, WM_LBUTTONDOWN, etc.                
                case SDL_EVENT_MOUSE_WHEEL:                    
                    InternalSdl::g_MouseWheel += event.wheel.integer_y;
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
