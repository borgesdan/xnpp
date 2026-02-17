#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Game/GameHost.hpp"
#include "InternalSdl.hpp"
#include <SDL3/SDL.h>

namespace Xna {    
    static void HandleGamepadAdded(SDL_Event const& event, std::vector<SdlGamePadPlayer>& gamePadPlayers);
    static void HandleGamepadRemoved(SDL_Event const& event, std::vector<SdlGamePadPlayer>& gamePadPlayers);
    static void CloseAllGamepads(std::vector<SdlGamePadPlayer>& gamePadPlayers);

	void Platform::GameHost_Tick(GameHost& gh) {
        SDL_Event event;
        bool running = true;
        std::vector<SdlGamePadPlayer> gamePadPlayers(4);
        
        if (!SDL_WasInit(SDL_INIT_GAMEPAD))
            SDL_Init(SDL_INIT_GAMEPAD);

        //AddConnectedGamePads(gamePadPlayers);

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

                    //WM_MOUSEMOVE, WM_LBUTTONDOWN, etc.                
                case SDL_EVENT_MOUSE_WHEEL:                    
                    InternalSdl::g_MouseWheel += event.wheel.integer_y;
                    break;
                case SDL_EVENT_GAMEPAD_ADDED:
                    HandleGamepadAdded(event, gamePadPlayers);
                    break;
                case SDL_EVENT_GAMEPAD_REMOVED:
                    HandleGamepadRemoved(event, gamePadPlayers);
                    break;
                }                
            }
            
            if (gh.impl->exitRequested || !running) 
                gh.impl->gameWindow.Close();
            else                 
                gh.RunOneFrame();
        }

        //TODO: [!] Mover para uma área de Dispose
        CloseAllGamepads(gamePadPlayers);
	}   

    void HandleGamepadAdded(SDL_Event const& event, std::vector<SdlGamePadPlayer>& gamePadPlayers)
    {
        auto newPad = SDL_OpenGamepad(event.gdevice.which);

        if (newPad) {       
            for (int i = 0; i < gamePadPlayers.size(); ++i) {
                if (gamePadPlayers[i].gamepad == nullptr) {
                    gamePadPlayers[i].gamepad = newPad;
                    gamePadPlayers[i].id = i;

                    //Define o Player Index no SDL
                    SDL_SetGamepadPlayerIndex(newPad, i);
                    
                    SDL_Log("Gamepad %d connected: %s", i + 1, SDL_GetGamepadName(newPad));
                    break;
                }
            }
        }
    }    

    void HandleGamepadRemoved(SDL_Event const& event, std::vector<SdlGamePadPlayer>& gamePadPlayers) {
        SDL_Gamepad* removedPad = SDL_GetGamepadFromID(event.gdevice.which);

        if (removedPad) {
            for (int i = 0; i < gamePadPlayers.size(); ++i) {
                if (gamePadPlayers[i].gamepad == removedPad) {
                    SDL_Log("Gamepad %d desconnected.", i + 1);                    
                    SDL_CloseGamepad(gamePadPlayers[i].gamepad);

                    gamePadPlayers[i].gamepad = nullptr;
                    gamePadPlayers[i].id = -1;

                    break;
                }
            }
        }
    }

    void CloseAllGamepads(std::vector<SdlGamePadPlayer>& gamePadPlayers) {
        for (auto& player : gamePadPlayers) {
            if (player.gamepad) {
                SDL_CloseGamepad(player.gamepad);
                player.gamepad = nullptr;
                player.id = -1;
            }
        }

        SDL_Log("The pending gamepads have been disconnected.");
    }
}
