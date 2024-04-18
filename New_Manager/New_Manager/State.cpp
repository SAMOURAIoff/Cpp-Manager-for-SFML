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
}

void Test::update()
{
	time = timerr.getElapsedTime();

	if(time.asSeconds() >= 5)
	{
		GET_MANAGER->getSteam().getServeur().searchLobby();
		GET_MANAGER->getSteam().getServeur().getNumLobbies();
		GET_MANAGER->getSteam().getServeur().connectRandomLobby();
		timerr.restart();
	}


    if (KEY(R) and m_windowManager.timer() > 0.2f)
    {
        m_windowManager.resetTimer();
        GET_MANAGER->loadScene("Test");
        pushState(1);
    }

	

	if (GET_MANAGER->getSteam().getManette().get_analog_action("Move").y > 0.3f)
	{
		posPlayer.y -= 0.1;
	}
	if (GET_MANAGER->getSteam().getManette().get_analog_action("Move").y < -0.3f)
	{
		posPlayer.y += 0.1;
	}
	if (GET_MANAGER->getSteam().getManette().get_analog_action("Move").x < -0.3f)
	{
		posPlayer.x -= 0.1;
	}
	if (GET_MANAGER->getSteam().getManette().get_analog_action("Move").x > 0.3f)
	{
		posPlayer.x += 0.1;
	}

	if (GET_MANAGER->getSteam().getManette().get_analog_action("Camera").y > 0.3f)
	{
		posPlayer2.y -= 0.1;
	}
	if (GET_MANAGER->getSteam().getManette().get_analog_action("Camera").y < -0.3f)
	{
		posPlayer2.y += 0.1;
	}
	if (GET_MANAGER->getSteam().getManette().get_analog_action("Camera").x < -0.3f)
	{
		posPlayer2.x -= 0.1;
	}
	if (GET_MANAGER->getSteam().getManette().get_analog_action("Camera").x > 0.3f)
	{
		posPlayer2.x += 0.1;
	}

	player.setPosition(posPlayer);
	player2.setPosition(posPlayer2);

	static bool test = false;

	if (GET_MANAGER->getSteam().getManette().get_button_action("A").bState == true)
	{
		GET_MANAGER->getSteam().getManette().setVibration(1000, 1000);
		test = true;
	}
	else
	{
		if (test)
		{
			GET_MANAGER->getSteam().getManette().setVibration(0, 0);
		}
		
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
