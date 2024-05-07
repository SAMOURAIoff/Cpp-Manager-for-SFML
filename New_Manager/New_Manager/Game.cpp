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

    const std::string filename = "test_data.txt";
    const std::string dataToSave = "suuu";y
    const int bufferSize = 256;
    char buffer[bufferSize];

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

        if(KEY(A))
        {
            if (cloudManager.saveDataToCloud(filename, dataToSave.c_str(), dataToSave.size()))
                std::cout << "Donnees enregistrees dans le Cloud avec succes." << std::endl;
            else
                std::cerr << "Erreur lors de l'enregistrement des donnees dans le Cloud." << std::endl;
        }

        if(KEY(Z))
        {
            const int bytesRead = cloudManager.loadDataFromCloud(filename, buffer, bufferSize);
            if (bytesRead > 0) 
                std::cout << "Donnees chargees depuis le Cloud : " << std::string(buffer, bytesRead) << std::endl;
            else 
                std::cerr << "Erreur lors du chargement des donnees depuis le Cloud." << std::endl;
        }

        if(KEY(E))
        {
            if (cloudManager.isCloudEnabled()) 
                std::cout << "Le cloud ai activer" << std::endl;
            else 
                std::cout << "Le cloud n'ai pas activer" << std::endl;
        }
        
        if(KEY(R))
        {
            if (cloudManager.isCloudFileExists(filename)) 
                std::cout << "Le fichier '" << filename << "' existe dans le Cloud." << std::endl;
            else 
                std::cout << "Le fichier '" << filename << "' n'existe pas dans le Cloud." << std::endl;
        }
        
        if(KEY(T))
        {
            if (cloudManager.deleteCloudFile(filename)) 
                std::cout << "Le fichier '" << filename << "' a ete supprime du Cloud." << std::endl;
            else
                std::cerr << "Erreur lors de la suppression du fichier '" << filename << "' du Cloud." << std::endl;
        }

        if(KEY(Y))
        {
            std::vector<std::pair<std::string, int32>> files = cloudManager.listCloudFiles();

            // Affichage des fichiers de sauvegarde
            std::cout << "Fichiers de sauvegarde pour l'utilisateur : " << std::endl;
            for (const auto& file : files) {
                std::cout << "- Nom : " << file.first << ", Taille : " << file.second << " octets" << std::endl;
            }
        }
          

        render();
    }

    GET_MANAGER->getSteam().~SteamManager();
}