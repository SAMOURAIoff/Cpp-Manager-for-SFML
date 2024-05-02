#include "Game.h"
#include "SFML_ENGINE/RessourcesManager.h"


Game::Game() : m_windowManager()
{
}

Game::Game(int width, int height, std::string title, bool fullscreen) : m_windowManager(width,height,title,fullscreen)
{
}

Game::~Game()
{
    while (!m_state.empty())
        m_state.pop();
}

void Game::update()
{
    Tools::restartClock();

    m_windowManager.EventUpdate();
	if (!m_state.empty() and GET_MANAGER->IsReady())
		m_state.top().get()->update();

}

void Game::render()
{
    m_windowManager.clear();
    if (!m_state.empty() and GET_MANAGER->IsReady())
        m_state.top().get()->render();
    else if (!GET_MANAGER->IsReady())
        GET_MANAGER->showLoadingScreen(m_windowManager.getWindow());
    m_windowManager.display();
}

void Game::runGame()
{
    GET_MANAGER->getSteam(); 
    GET_MANAGER->getSteam().getManette().init("GameControls");
    achievmentHandle.initFromSteamworks();
    achievmentHandle.request();

    // Création des objets CCallback pour les fonctions de rappel
    CCallback<SteamServersConnected_t, MySteamCallbacks> callbackConnected(&MySteamCallbacks::SteamServersConnectedCallback);
    CCallback<SteamServersDisconnected_t, MySteamCallbacks> callbackDisconnected(&MySteamCallbacks::SteamServersDisconnectedCallback);

    // Enregistrement des fonctions de rappel pour les événements Steam
    SteamAPI_RegisterCallback(callbackConnected, SteamServersConnected_t::k_iCallback);
    SteamAPI_RegisterCallback(callbackDisconnected, SteamServersDisconnected_t::k_iCallback);


    GET_MANAGER->getLoadingScreen() = Animation(GET_MANAGER->getTexture("loading"), sf::IntRect(0, 0, 128, 128), 0.1f, 8);
    m_state.push(std::make_unique<Test>(m_windowManager, &m_state));
    while (!m_windowManager.isDone())
    {
        GET_MANAGER->setVolumeForAllMusic(m_windowManager.getMusic_Volume());
        GET_MANAGER->setVolumeForAllSound(m_windowManager.getSFX_Volume());
        GET_MANAGER->setVolumeForAllMovie(m_windowManager.getMovie_Volume());
        while (m_state.size() > 1)
            m_state.pop();

        GET_MANAGER->getSteam().update();

        update();

        // Définition et récupération de la valeur d'une statistique
        achievmentHandle.setStat("stat1", 10);
        int value;
        if (achievmentHandle.getStat("stat1", &value)) {
            std::cout << "Stat1 value: " << value << std::endl;
        }

        // Définition de la valeur d'un succès
        if (achievmentHandle.setAchievement("ACHIEVEMENT1")) {
            std::cout << "Achievement1 unlocked!" << std::endl;
        }

        // Réinitialisation d'un succès
        if (achievmentHandle.clearAchievement("ACHIEVEMENT1")) {
            std::cout << "Achievement1 reset!" << std::endl;
        }

        // Mise à jour des statistiques et des succès
        achievmentHandle.storeStats();

        render();
    }

    GET_MANAGER->getSteam().~SteamManager();
}