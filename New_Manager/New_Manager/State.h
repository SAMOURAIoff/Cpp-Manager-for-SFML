/*
    Based on Coffee Engine

    Author: GRALLAN Yann

    Description: SFML-Engine

    Date: 2024/01/31

    MIT License

    Copyright (c) 2024 GRALLAN Yann


    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once
#include "SFML_ENGINE/WindowManager.h"
#include "SFML_ENGINE/Sprite.h"
#include "steam.h"


class State
{
protected:
	std::stack<std::unique_ptr<State>>* m_stackState;
	WindowManager& m_windowManager;

    SteamManager m_steam;

public:
	State(WindowManager& _window, std::stack<std::unique_ptr<State>>* stackState);
	void virtual update() = 0;
    void virtual render() = 0;
    void virtual pushState(char data) = 0;
};

typedef std::stack<std::unique_ptr<State>> StateStack;

class Test : public State
{
private:
    Animation m_animation;
	void virtual update();
    void virtual render();
    void virtual pushState(char data);
public:
	Test(WindowManager& _window, StateStack* stackState);

    sf::RectangleShape player;
    sf::Vector2f posPlayer;
    sf::RectangleShape player2;
    sf::Vector2f posPlayer2;
    sf::Clock timerr;
    sf::Time time;
};

