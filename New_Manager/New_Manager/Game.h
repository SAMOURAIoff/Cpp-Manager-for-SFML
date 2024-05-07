
#pragma once
#include "State.h"
#include "SFML_ENGINE/SteamManager.h"

#include "AchievmentHandle.h"
#include "CloudHandle.h"

class Game
{

    AchievmentHandle achievmentHandle;
	WindowManager m_windowManager;
	StateStack m_state;
	CloudManager cloudManager;

    void update();
    void render();
public:
	Game();
    Game(int width, int height, std::string title, bool fullscreen);
    ~Game();

    void runGame();

    
};

//class MySteamCallbacks {
//public:
//    static void SteamServersConnectedCallback(SteamServersConnected_t* pCallback) {
//        // L'utilisateur est connect� � Steam
//        std::cout << "Connected to Steam." << std::endl;
//    }
//
//    static void SteamServersDisconnectedCallback(SteamServersDisconnected_t* pCallback) {
//        // L'utilisateur est d�connect� de Steam
//        std::cout << "Disconnected from Steam. Reason: " << pCallback->m_eResult << std::endl;
//    }
//};
