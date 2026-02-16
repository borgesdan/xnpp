#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Game/GameHost.hpp"
#include "InternalSdl.hpp"
#include <SDL3/SDL.h>

namespace Xna {    
    static void HandleGamepadAdded(int deviceIndex);
    static void HandleGamepadRemoved(SDL_JoystickID instanceID);

	void Platform::GameHost_Tick(GameHost& gh) {
        SDL_Event event;
        bool running = true;
        
        if (!SDL_WasInit(SDL_INIT_GAMEPAD))
            SDL_Init(SDL_INIT_GAMEPAD);

        //Initialize connected gamepads
        int gamePadCount;
        SDL_GetGamepads(&gamePadCount);

        for (int i = 0; i < gamePadCount; ++i)
            HandleGamepadAdded(i);

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
                    HandleGamepadAdded(event.gdevice.which);
                    break;
                case SDL_EVENT_GAMEPAD_REMOVED:
                    HandleGamepadRemoved(event.gdevice.which);
                    break;
                }                
            }
            
            if (gh.impl->exitRequested || !running) 
                gh.impl->gameWindow.Close();
            else                 
                gh.RunOneFrame();
        }
	}

    void HandleGamepadAdded(int deviceIndex)
    {
        if (!SDL_IsGamepad(deviceIndex))
            return;

        auto pad = SDL_OpenGamepad(deviceIndex);
        if (!pad)
            return;

        const auto instanceID = SDL_GetGamepadID(pad);
       
        for (size_t i = 0; i < InternalSdl::MAX_GAMEPADS; ++i)
        {
            if (!InternalSdl::g_Gamepads[i])
            {
                InternalSdl::g_Gamepads[i] = pad;
                InternalSdl::g_InstanceIDs[i] = instanceID;
                return;
            }
        }

        // Sem slot livre
        SDL_CloseGamepad(pad);
    }

    void HandleGamepadRemoved(SDL_JoystickID instanceID) {
        for (size_t i = 0; i < InternalSdl::MAX_GAMEPADS; ++i)
        {
            if (InternalSdl::g_Gamepads[i] && InternalSdl::g_InstanceIDs[i] == instanceID)
            {
                SDL_CloseGamepad(InternalSdl::g_Gamepads[i]);
                InternalSdl::g_Gamepads[i] = nullptr;
                InternalSdl::g_InstanceIDs[i] = 0;
                return;
            }
        }
    }
}
