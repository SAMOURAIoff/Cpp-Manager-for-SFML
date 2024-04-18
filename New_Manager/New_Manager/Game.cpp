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
    
    GET_MANAGER->getLoadingScreen() = Animation(GET_MANAGER->getTexture("loading"), sf::IntRect(0, 0, 128, 128), 0.1f, 8);
    m_state.push(std::make_unique<Test>(m_windowManager, &m_state));
    while (!m_windowManager.isDone())
    {
        GET_MANAGER->setVolumeForAllMusic(m_windowManager.getMusic_Volume());
        GET_MANAGER->setVolumeForAllSound(m_windowManager.getSFX_Volume());
        GET_MANAGER->setVolumeForAllMovie(m_windowManager.getMovie_Volume());
        while (m_state.size() > 1)
            m_state.pop();


        update();
        render();
    }

}