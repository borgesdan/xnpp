#include "SdlGameHost.hpp"

namespace Xna {
    static constexpr Uint32 GAMEPAD_FRAME_RUMBLE_MS = 16;
    static constexpr size_t MAX_GAME_PADS = 4;

    static bool SuspendGamepads = false;
    static int MouseWheel = 0;
    static std::vector<GamepadPlayer> Gamepads = std::vector<GamepadPlayer>(MAX_GAME_PADS);

    static void HandleGamepadAdded(SDL_Event const& event, std::vector<GamepadPlayer>& gamePadPlayers);
    static void HandleGamepadRemoved(SDL_Event const& event, std::vector<GamepadPlayer>& gamePadPlayers);
    static void CloseAllGamepads(std::vector<GamepadPlayer>& gamePadPlayers);
    static void UpdateGamePads(std::vector<GamepadPlayer>& gamePadPlayers);

    void SdlGameHost::Tick() {
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
                    SuspendGamepads = false;
                    break;
                case SDL_EVENT_WINDOW_FOCUS_LOST:
                    SuspendGamepads = true;
                    break;

                    //WM_MOUSEMOVE, WM_LBUTTONDOWN, etc.                
                case SDL_EVENT_MOUSE_WHEEL:
                    MouseWheel += event.wheel.integer_y;
                    break;
                case SDL_EVENT_GAMEPAD_ADDED:
                    HandleGamepadAdded(event, Gamepads);
                    break;
                case SDL_EVENT_GAMEPAD_REMOVED:
                    HandleGamepadRemoved(event, Gamepads);
                    break;
                }
            }

            if (ExitRequested() || !running)
                Window()->Close();
            else {
                UpdateGamePads(Gamepads);
                RunOneFrame();
            }
        }

        //TODO: [!] Mover para uma área de Dispose
        CloseAllGamepads(Gamepads);
    }

    void UpdateGamePads(std::vector<GamepadPlayer>& gamePadPlayers) {

        if (SuspendGamepads) {
            for (auto& player : gamePadPlayers) {
                SDL_RumbleGamepad(player.gamepad, 0, 0, 0);
                SDL_RumbleGamepadTriggers(player.gamepad, 0, 0, 0);
            }
        }

        for (auto& player : gamePadPlayers) {
            if (!player.gamepad)
                continue;

            if (player.high_frequency_rumble > 0 || player.low_frequency_rumble > 0) {
                SDL_RumbleGamepad(
                    player.gamepad,
                    player.low_frequency_rumble,
                    player.high_frequency_rumble,
                    GAMEPAD_FRAME_RUMBLE_MS);
            }
            else {
                SDL_RumbleGamepad(player.gamepad, 0, 0, 0);
            }

            if (player.left_rumble > 0 || player.right_rumble > 0) {
                SDL_RumbleGamepadTriggers(
                    player.gamepad,
                    player.left_rumble,
                    player.right_rumble,
                    GAMEPAD_FRAME_RUMBLE_MS);
            }
            else {
                SDL_RumbleGamepadTriggers(player.gamepad, 0, 0, 0);
            }
        }
    }

    void HandleGamepadAdded(SDL_Event const& event, std::vector<GamepadPlayer>& gamePadPlayers)
    {
        auto newPad = SDL_OpenGamepad(event.gdevice.which);

        if (newPad) {
            for (int i = 0; i < gamePadPlayers.size(); ++i) {
                if (gamePadPlayers[i].gamepad == nullptr) {
                    gamePadPlayers[i].gamepad = newPad;
                    gamePadPlayers[i].index = i;

                    //Define o Player Index no SDL
                    SDL_SetGamepadPlayerIndex(newPad, i);

                    SDL_Log("Gamepad %d connected: %s", i + 1, SDL_GetGamepadName(newPad));
                    break;
                }
            }
        }
    }

    void HandleGamepadRemoved(SDL_Event const& event, std::vector<GamepadPlayer>& gamePadPlayers) {
        SDL_Gamepad* removedPad = SDL_GetGamepadFromID(event.gdevice.which);

        if (removedPad) {
            for (int i = 0; i < gamePadPlayers.size(); ++i) {
                if (gamePadPlayers[i].gamepad == removedPad) {
                    SDL_Log("Gamepad %d desconnected.", i + 1);
                    SDL_CloseGamepad(gamePadPlayers[i].gamepad);

                    gamePadPlayers[i].gamepad = nullptr;
                    gamePadPlayers[i].index = -1;

                    break;
                }
            }
        }
    }

    void CloseAllGamepads(std::vector<GamepadPlayer>& gamePadPlayers) {
        for (auto& player : gamePadPlayers) {
            if (player.gamepad) {
                SDL_RumbleGamepad(player.gamepad, 0, 0, 0);
                SDL_RumbleGamepadTriggers(player.gamepad, 0, 0, 0);

                SDL_CloseGamepad(player.gamepad);
                player.gamepad = nullptr;
                player.index = -1;
            }
        }

        SDL_Log("The pending gamepads have been disconnected.");
    }
}