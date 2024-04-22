#include "State.h"
#include "SFML_ENGINE/RessourcesManager.h"


State::State(WindowManager& _window, std::stack<std::unique_ptr<State>>* stackState) : m_windowManager(_window), m_stackState(stackState)
{
}

void State::update()
{
}

Test::Test(WindowManager& _window, StateStack* stackState) : State(_window, stackState)
{
    m_animation = Animation(GET_MANAGER->getTexture("porte_intro_1224x855"),sf::IntRect(0,0,1224,855),0.5,16);

	player.setSize(sf::Vector2f(50,50));
	player.setPosition(sf::Vector2f(100, 100));
	player.setFillColor(sf::Color(255, 0, 0, 255));
	posPlayer = player.getPosition();

	player2.setSize(sf::Vector2f(50, 50));
	player2.setPosition(sf::Vector2f(500, 500));
	player2.setFillColor(sf::Color(0, 255, 0, 255));
	posPlayer2 = player2.getPosition();
	receivedData = 0;
	
}

void Test::update()
{
	m_steam.update();
	if (KEY(Numpad5) and m_windowManager.timer() > 0.2f)
	{
		m_steam.getServeur().connectP2PWithPlayersInLobby();								//lancer et connexion   5
		m_windowManager.resetTimer();
	}
	

	if (KEY(Numpad6) and m_windowManager.timer() > 0.2f)
	{
		m_steam.getServeur().checkAllP2PConnections();										//verifier les connexion une fois lancer  6
		m_windowManager.resetTimer();
	}

	if (KEY(Numpad8) and m_windowManager.timer() > 0.2f)
	{
		int dataToSend = 50;
		m_steam.getServeur().sendDataToOtherPlayers(&dataToSend, sizeof(int));              //envoie data  8
		m_windowManager.resetTimer();
	}

	
		m_steam.getServeur().receiveDataFromOtherPlayers(&receivedData, sizeof(int));           //recoie data  auto
	

	if (KEY(Numpad9) and m_windowManager.timer() > 0.4f)
	{
		std::cout << "Donnees reçues : " << receivedData << std::endl;                      // view data receive  9
		m_windowManager.resetTimer();
	}
		

	if(KEY(Numpad2) and m_windowManager.timer() > 0.2f)
	{
		m_steam.getServeur().searchLobby();
		m_steam.getServeur().connectRandomLobby();                                          //connection lobby  2
		m_windowManager.resetTimer();
	}

	if (KEY(Numpad1) and m_windowManager.timer() > 0.2f)
	{
		m_steam.getServeur().createLobby(k_ELobbyTypePublic,2);                              //creer lobby  1
		m_steam.getServeur().searchLobby();
		m_windowManager.resetTimer();
	}

	if (KEY(Numpad3) and m_windowManager.timer() > 0.2f)
	{
		m_steam.getServeur().searchLobby();
		m_steam.getServeur().getNumLobbies();                                               //verif nb lobby  3
		m_windowManager.resetTimer();
	}

    if (KEY(R) and m_windowManager.timer() > 0.2f)
    {
        m_windowManager.resetTimer();
        GET_MANAGER->loadScene("Test");
        pushState(1);
    }

	
}

void Test::render()
{
    m_animation.Animate(m_windowManager.getWindow(), 6);
	m_windowManager.draw(player);
	m_windowManager.draw(player2);
}

void Test::pushState(char data)
{
    if (data == 1)
        m_stackState->push(std::make_unique<Test>(m_windowManager, m_stackState));
}
