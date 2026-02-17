#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Game/GameHost.hpp"
#include "InternalSdl.hpp"
#include <SDL3/SDL.h>

namespace Xna {    
    static void HandleGamepadAdded(SDL_Event const& event, std::vector<SdlGamePadPlayer>& gamePadPlayers);
    static void HandleGamepadRemoved(SDL_Event const& event, std::vector<SdlGamePadPlayer>& gamePadPlayers);
    static void CloseAllGamepads(std::vector<SdlGamePadPlayer>& gamePadPlayers);
    static void UpdateGamePads(std::vector<SdlGamePadPlayer>& gamePadPlayers);

	void Platform::GameHost_Tick(GameHost& gh) {
        SDL_Event event;
        bool running = true;        
        
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
                    HandleGamepadAdded(event, Global::Gamepads);
                    break;
                case SDL_EVENT_GAMEPAD_REMOVED:
                    HandleGamepadRemoved(event, Global::Gamepads);
                    break;
                }                
            }
            
            if (gh.impl->exitRequested || !running) 
                gh.impl->gameWindow.Close();
            else {
                UpdateGamePads(Global::Gamepads);
                gh.RunOneFrame();
            }
        }       

        //TODO: [!] Mover para uma área de Dispose
        CloseAllGamepads(Global::Gamepads);
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
                SDL_RumbleGamepad(player.gamepad, 0, 0, 0);
                SDL_RumbleGamepadTriggers(player.gamepad, 0, 0, 0);

                SDL_CloseGamepad(player.gamepad);
                player.gamepad = nullptr;
                player.id = -1;
            }
        }

        SDL_Log("The pending gamepads have been disconnected.");
    }    

    void UpdateGamePads(std::vector<SdlGamePadPlayer>& gamePadPlayers) {
        static bool checkEnabled = false;

        if (!Global::GamepadsEnabled && !checkEnabled) {
            for (auto& player : gamePadPlayers) {
                SDL_RumbleGamepad(player.gamepad, 0, 0, 0);
                SDL_RumbleGamepadTriggers(player.gamepad, 0, 0, 0);
            }

            checkEnabled = true;
        }            

        checkEnabled = false;

        for (auto& player : gamePadPlayers) {
            if (!player.gamepad)
                continue;            

            if (player.high_frequency_rumble > 0 || player.low_frequency_rumble > 0) {
                SDL_RumbleGamepad(
                    player.gamepad, 
                    player.low_frequency_rumble,
                    player.high_frequency_rumble,
                    SdlGamePadPlayer::FRAME_RUMBLE_MS);
            }
            else {
                SDL_RumbleGamepad(player.gamepad, 0, 0, 0);
            }

            if (player.left_rumble > 0 || player.right_rumble > 0) {
                SDL_RumbleGamepadTriggers(
                    player.gamepad,
                    player.left_rumble,
                    player.right_rumble,
                    SdlGamePadPlayer::FRAME_RUMBLE_MS);
            }
            else {
                SDL_RumbleGamepadTriggers(player.gamepad, 0, 0, 0);
            }
        }
    }
}
