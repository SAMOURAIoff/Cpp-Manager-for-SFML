/*
	Steam Engine

	Author: DOS SANTOS Lucas

	Description: SFML-Engine / GRALLAN Yann

	Date: 2024/04/31

	MIT License

	Copyright (c) 2024 DOS SANTOS Lucas
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
#include "Tools.h"

class SFMLENGINE_API ManetteHandle
{
private:
	InputHandle_t m_manetteHandles[STEAM_INPUT_MAX_COUNT];///< Array of controller handles
	int m_nb_manette;///< Number of connected controllers
	InputActionSetHandle_t m_actionSet;///< Handle to the action set
	ScePadTriggerEffectParam param{};///< Parameters for controller vibration
	std::map<std::string, InputDigitalActionData_t> m_buttons_actions;///< Map of digital action data for buttons
	std::map<std::string, std::tuple<InputAnalogActionData_t, std::array<sf::Vector2f, 10>, unsigned short>> m_analog_actions;///< Map of analog action data for analog inputs
	std::map<std::string, InputActionSetHandle_t> m_actions;///< Map of action set handles
	std::string m_bind_group_name;///< Name of the controller bind group
	bool m_rebind_controller;///< Flag indicating whether controller rebinding is active

public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	////////////////////////////////////////////////////////////
	ManetteHandle();

	////////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	////////////////////////////////////////////////////////////
	~ManetteHandle();

	////////////////////////////////////////////////////////////
	/// \brief Initialize the controller handle with a specific bind group
	/// \param _bind_group_name The name of the bind group to use
	////////////////////////////////////////////////////////////
	void init(std::string _bind_group_name);

	////////////////////////////////////////////////////////////
	/// \brief Update controller input state
	///
	////////////////////////////////////////////////////////////
	void update ();

	////////////////////////////////////////////////////////////
	/// \brief Set vibration intensity for the controller
	/// \param usLeftSpeed Intensity of the left motor
	/// \param usRightSpeed Intensity of the right motor
	////////////////////////////////////////////////////////////
	void setVibration(unsigned short usLeftSpeed, unsigned short usRightSpeed) const;

	////////////////////////////////////////////////////////////
	/// \brief Set dualsense trigger effect for the controller (left & right)
	/// \param start position where the stiffness of trigger start changing(2~7)
	/// \param end position where the stiffness of trigger finish changing(startPosition+1~8)
	/// \param strenght  of gun trigger(0~8 (0: Same as Off mode))
	////////////////////////////////////////////////////////////
	void setDualSenseTriggerEffect(int l_start, int l_end, int l_strenght, int r_start, int r_end, int r_strenght);

	////////////////////////////////////////////////////////////
	/// \brief Create an analog action for the controller
	/// \param _action The name of the action/analog
	////////////////////////////////////////////////////////////
	void create_analog_action(std::string _action);

	////////////////////////////////////////////////////////////
	/// \brief Create a button action for the controller
	/// \param _action The name of the action/button
	////////////////////////////////////////////////////////////
	void create_button_action(std::string _action);

	////////////////////////////////////////////////////////////
	/// \brief Get the number of connected controllers
	/// \return Reference to the number of connected controllers
	////////////////////////////////////////////////////////////
	inline int& getNbController() { return m_nb_manette; }

	////////////////////////////////////////////////////////////
	/// \brief Get the array of controller handles
	/// \return Pointer to the array of controller handles
	////////////////////////////////////////////////////////////
	inline InputHandle_t* getHandleController() { return m_manetteHandles; }

	////////////////////////////////////////////////////////////
	/// \brief Get the analog action data for a specific action
	/// \param _action The name of the action
	/// \return Reference to the analog action data
	////////////////////////////////////////////////////////////
	InputAnalogActionData_t& get_analog_action(std::string _action) { return std::get<0>(m_analog_actions[_action]); }

	////////////////////////////////////////////////////////////
	/// \brief Get the digital action data for a specific action
	/// \param _action The name of the action
	/// \return Reference to the digital action data
	////////////////////////////////////////////////////////////
	InputDigitalActionData_t& get_button_action(std::string _action) { return m_buttons_actions[_action]; }
};


struct LobbyCreated_t;
struct LobbyMatchList_t;

class SFMLENGINE_API LobbyHandle
{
private:
	bool m_connectedToLobby;
	CSteamID m_currentLobby;
	int m_numLobbies;

	CCallResult< LobbyHandle, LobbyMatchList_t> m_CallbackLobbyDataUpdated;
	typedef void (LobbyHandle::* LobbyDataCallback_t)(LobbyMatchList_t*, bool);

	CCallback<LobbyHandle, LobbyCreated_t> m_CallbackCreateLobby;
	typedef void (LobbyHandle::* CallbackFunc_t)(LobbyCreated_t*, bool);

public:
	LobbyHandle();
	~LobbyHandle();

	void createLobby(ELobbyType LobbyType, int MaxMembers);

	void searchLobby();
	void inviteFriendtoLobby(CSteamID playerSteamID);
	void connectToLobby(CSteamID remoteSteamID);
	bool connectRandomLobby();
	void disconnectLobby();
	bool isConnectedToLobby();
	int getNumLobbies();
	CSteamID getCureentLobby();

	void OnLobbyDataUpdated(LobbyMatchList_t* pCallback, bool);
	void OnLobbyCreated(LobbyCreated_t* pParam);

};

class SFMLENGINE_API SteamManager
{
private:
	ManetteHandle m_manetteH;
	LobbyHandle m_lobbyH;

public:
	SteamManager();
	void update();
	~SteamManager();
	
	ManetteHandle& getManette();
	LobbyHandle& getLobby();

};
