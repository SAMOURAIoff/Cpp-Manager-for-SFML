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
	int m_nbManette;///< Number of connected controllers
	InputActionSetHandle_t m_actionSet;///< Handle to the action set
	ScePadTriggerEffectParam param{};///< Parameters for controller vibration
	std::map<std::string, InputDigitalActionData_t> m_buttonsActions;///< Map of digital action data for buttons
	std::map<std::string, std::tuple<InputAnalogActionData_t, std::array<sf::Vector2f, 10>, unsigned short>> m_analogActions;///< Map of analog action data for analog inputs
	std::map<std::string, InputActionSetHandle_t> m_actions;///< Map of action set handles
	std::string m_bindGroupName;///< Name of the controller bind group
	bool m_rebindController;///< Flag indicating whether controller rebinding is active

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
	/// \param bindGroupName The name of the bind group to use
	////////////////////////////////////////////////////////////
	void init(std::string bindGroupName);

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
	/// \param l_start position where the stiffness of trigger start changing(2~7)
	/// \param l_end position where the stiffness of trigger finish changing(startPosition+1~8)
	/// \param l_strenght  of gun trigger(0~8 (0: Same as Off mode))
	////////////////////////////////////////////////////////////
	void setDualSenseTriggerEffect(uint8_t l_start, uint8_t l_end, uint8_t l_strenght, uint8_t r_start, uint8_t r_end, uint8_t r_strenght);

	////////////////////////////////////////////////////////////
	/// \brief Create an analog action for the controller
	/// \param action The name of the action/analog
	////////////////////////////////////////////////////////////
	void create_analog_action(std::string action);

	////////////////////////////////////////////////////////////
	/// \brief Create a button action for the controller
	/// \param action The name of the action/button
	////////////////////////////////////////////////////////////
	void create_button_action(std::string action);

	////////////////////////////////////////////////////////////
	/// \brief Get the number of connected controllers
	/// \return Reference to the number of connected controllers
	////////////////////////////////////////////////////////////
	inline int& getNbController() { return m_nbManette; }

	////////////////////////////////////////////////////////////
	/// \brief Get the array of controller handles
	/// \return Pointer to the array of controller handles
	////////////////////////////////////////////////////////////
	inline InputHandle_t* getHandleController() { return m_manetteHandles; }

	////////////////////////////////////////////////////////////
	/// \brief Get the analog action data for a specific action
	/// \param action The name of the action
	/// \return Reference to the analog action data
	////////////////////////////////////////////////////////////
	InputAnalogActionData_t& get_analog_action(std::string action) { return std::get<0>(m_analogActions[action]); }

	////////////////////////////////////////////////////////////
	/// \brief Get the digital action data for a specific action
	/// \param action The name of the action
	/// \return Reference to the digital action data
	////////////////////////////////////////////////////////////
	InputDigitalActionData_t& get_button_action(std::string action) { return m_buttonsActions[action]; }
};

class SFMLENGINE_API AchievmentHandle
{
private:
	CSteamID m_steamID;///< Steam ID of the user
	
public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	////////////////////////////////////////////////////////////
	AchievmentHandle();
	
	////////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	////////////////////////////////////////////////////////////
	~AchievmentHandle() = default;

	////////////////////////////////////////////////////////////
	/// \brief Initializes achievements and stats from Steamworks.
	/// \return True if initialization is successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool initFromSteamworks();

	////////////////////////////////////////////////////////////
	/// \brief Sets the value of a stat.
	/// \param name The name of the stat.
	/// \param value The value to set.
	/// \return True if successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool setStat(const char* name, int value);

	////////////////////////////////////////////////////////////
	/// \brief Gets the value of a stat.
	/// \param name The name of the stat.
	/// \param value Pointer to store the value.
	/// \return True if successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool getStat(const char* name, int* value);

	////////////////////////////////////////////////////////////
	/// \brief Increments the value of a stat.
	/// \param name The name of the stat.
	/// \param increment The amount to increment by.
	/// \return True if successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool incrementStat(const char* name, int increment);

	////////////////////////////////////////////////////////////
	/// \brief Stores stats on the Steam server.
	/// \return True if successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool storeStats();

	////////////////////////////////////////////////////////////
	/// \brief Sets an achievement.
	/// \param name The name of the achievement.
	/// \return True if successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool setAchievement(const char* name);

	////////////////////////////////////////////////////////////
	/// \brief Clears an achievement.
	/// \param name The name of the achievement.
	/// \return True if successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool clearAchievement(const char* name);
	
	////////////////////////////////////////////////////////////
	/// \brief Requests current stats from Steam.
	/// \return True if successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool request();

};

struct LobbyCreated_t;
struct LobbyMatchList_t;
class SFMLENGINE_API LobbyHandle
{
private:
	bool m_connectedToLobby;///< Flag indicating whether connected to a lobby
	CSteamID m_currentLobby;///< ID of the current lobby
	int m_numLobbies;///< Number of lobbies

	CCallResult< LobbyHandle, LobbyMatchList_t> m_CallbackLobbyDataUpdated;///< Callback for lobby data update
	typedef void (LobbyHandle::* LobbyDataCallback_t)(LobbyMatchList_t*, bool);

	CCallback<LobbyHandle, LobbyCreated_t> m_CallbackCreateLobby;///< Callback for lobby creation
	typedef void (LobbyHandle::* CallbackFunc_t)(LobbyCreated_t*, bool);

public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	////////////////////////////////////////////////////////////
	LobbyHandle();

	////////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	////////////////////////////////////////////////////////////
	~LobbyHandle();

	////////////////////////////////////////////////////////////
	/// \brief Creates a lobby.
	/// \param LobbyType The type of lobby.
	/// \param MaxMembers Maximum number of members in the lobby.
	////////////////////////////////////////////////////////////
	void createLobby(ELobbyType LobbyType, int MaxMembers);

	////////////////////////////////////////////////////////////
	/// \brief Searches for lobbies.
	/// 
	////////////////////////////////////////////////////////////
	void searchLobby();

	////////////////////////////////////////////////////////////
	/// \brief Invites a friend to the lobby.
	/// \param playerSteamID Steam ID of the friend.
	////////////////////////////////////////////////////////////
	void inviteFriendtoLobby(CSteamID playerSteamID);

	////////////////////////////////////////////////////////////
	/// \brief Connects to a specific lobby.
	/// \param remoteSteamID Steam ID of the remote lobby.
	////////////////////////////////////////////////////////////
	void connectToLobby(CSteamID remoteSteamID);

	////////////////////////////////////////////////////////////
	/// \brief Connects to a random lobby.
	/// \return True if successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool connectRandomLobby();

	////////////////////////////////////////////////////////////
	/// \brief Disconnects from the current lobby.
	////////////////////////////////////////////////////////////
	void disconnectLobby();

	////////////////////////////////////////////////////////////
	/// \brief Checks if connected to a lobby.
	/// \return True if connected to a lobby, false otherwise.
	////////////////////////////////////////////////////////////
	bool isConnectedToLobby();

	////////////////////////////////////////////////////////////
	/// \brief Gets the number of lobbies.
	/// \return Number of lobbies.
	////////////////////////////////////////////////////////////
	int getNumLobbies();
	
	////////////////////////////////////////////////////////////
	/// \brief Gets the ID of the current lobby.
	/// \return ID of the current lobby.
	////////////////////////////////////////////////////////////
	CSteamID getCureentLobby();

	////////////////////////////////////////////////////////////
	/// \brief Callback function for lobby data update.
	/// \param pCallback Pointer to the lobby data.
	////////////////////////////////////////////////////////////
	void OnLobbyDataUpdated(LobbyMatchList_t* pCallback, bool);

	////////////////////////////////////////////////////////////
	/// \brief Callback function for lobby creation.
	/// \param pParam Pointer to the lobby creation parameters.
	////////////////////////////////////////////////////////////
	void OnLobbyCreated(LobbyCreated_t* pParam);

};

class SFMLENGINE_API CloudHanle
{
public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor.
	/// 
	////////////////////////////////////////////////////////////
	CloudHanle();

	////////////////////////////////////////////////////////////
	/// \brief Destructor.
	/// 
	////////////////////////////////////////////////////////////
	~CloudHanle() = default;

	////////////////////////////////////////////////////////////////
	/// \brief Saves data to the Steam Cloud.
	/// \param filename The name of the file to save.
	/// \param data Pointer to the data to save.
	/// \param dataSize Size of the data to save.
	/// \return True if the data was saved successfully, false otherwise.
	////////////////////////////////////////////////////////////////
	bool saveDataToCloud(const std::string& filename, const void* data, int dataSize) ;

	////////////////////////////////////////////////////////////////
	/// \brief Loads data from the Steam Cloud.
	/// \param filename The name of the file to load.
	/// \param buffer Pointer to the buffer to store the loaded data.
	/// \param bufferSize Size of the buffer.
	/// \return The size of the loaded data, or -1 if the file does not exist.
	////////////////////////////////////////////////////////////////
	int loadDataFromCloud(const std::string& filename, void* buffer, int bufferSize);

	////////////////////////////////////////////////////////////////
	/// \brief Checks if the Steam Cloud is enabled for the game.
	/// \return True if the Steam Cloud is enabled, false otherwise.
	////////////////////////////////////////////////////////////////
	bool isCloudEnabled();

	////////////////////////////////////////////////////////////////
	/// \brief Checks if a specific file exists in the Steam Cloud.
	/// \param filename The name of the file to check.
	/// \return True if the file exists, false otherwise.
	////////////////////////////////////////////////////////////////
	bool isCloudFileExists(const std::string& filename);

	////////////////////////////////////////////////////////////////
	/// \brief Deletes a file from the Steam Cloud.
	/// \param filename The name of the file to delete.
	/// \return True if the file was deleted successfully, false otherwise.
	////////////////////////////////////////////////////////////////
	bool deleteCloudFile(const std::string& filename);
	
	////////////////////////////////////////////////////////////////
	/// \brief Lists all files in the Steam Cloud.
	/// \return A vector of pairs containing the filenames and their sizes.
	////////////////////////////////////////////////////////////////
	std::vector<std::pair<std::string, int32>> listCloudFiles();
    
};

class SFMLENGINE_API SteamManager
{
private:
	ManetteHandle m_manetteH;///< Handle for controller input
	AchievmentHandle m_achievmentH;///< Handle for achievements
	LobbyHandle m_lobbyH;///< Handle for lobbies
	CloudHanle m_cloudH;///< Handle for cloud

public:
	////////////////////////////////////////////////////////////
	/// \brief Default constructor.
	/// 
	////////////////////////////////////////////////////////////
	SteamManager();

	////////////////////////////////////////////////////////////
	/// \brief Updates Steam-related operations.
	/// 
	////////////////////////////////////////////////////////////
	void update();

	////////////////////////////////////////////////////////////
	/// \brief Destructor.
	/// 
	////////////////////////////////////////////////////////////
	~SteamManager();

	////////////////////////////////////////////////////////////
	/// \brief
	/// 
	////////////////////////////////////////////////////////////
	std::string getSteamLangage();

	////////////////////////////////////////////////////////////
	/// \brief 
	/// 
	////////////////////////////////////////////////////////////
	std::string getAllSteamLangage();

	////////////////////////////////////////////////////////////
	/// \brief Gets the controller handle.
	/// \return Reference to the controller handle.
	////////////////////////////////////////////////////////////
	ManetteHandle& getManette();
	
	////////////////////////////////////////////////////////////
	/// \brief Gets the achievement handle.
	/// \return Reference to the achievement handle.
	////////////////////////////////////////////////////////////
	AchievmentHandle& getAchievment();

	////////////////////////////////////////////////////////////
	/// \brief Gets the lobby handle.
	/// \return Reference to the lobby handle.
	////////////////////////////////////////////////////////////
	LobbyHandle& getLobby();

	////////////////////////////////////////////////////////////
	/// \brief Gets the cloud handle.
	/// \return Reference to the cloud handle.
	////////////////////////////////////////////////////////////
	CloudHanle& getCloud();
};