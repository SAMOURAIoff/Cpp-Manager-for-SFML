#include "SteamManager.h"

#pragma region API

SteamManager::SteamManager()
{
    std::cout << "------- Starting steam API load ------- \n";

    if (SteamAPI_Init())
	{
		SteamInput()->Init(true);

		m_lobbyH.searchLobby();
	}

    std::cout << "------- Steam API loading finish ------- \n\n\n";
}
void SteamManager::update()
{
    SteamAPI_RunCallbacks();
	m_manetteH.update();
}
SteamManager::~SteamManager()
{
    SteamAPI_Shutdown();
}

std::string SteamManager::getSteamLangage()
{
	const char* language = SteamApps()->GetCurrentGameLanguage();
	std::string steamLanguage(language);
	return steamLanguage;
}

std::string SteamManager::getAllSteamLangage()
{
	const char* language = SteamApps()->GetAvailableGameLanguages();
	std::string steamLanguage(language);
	return steamLanguage;
}

ManetteHandle& SteamManager::getManette()
{
	return m_manetteH;
}

AchievmentHandle& SteamManager::getAchievment()
{
	return m_achievmentH;
}

LobbyHandle& SteamManager::getLobby()
{
	return m_lobbyH;
}

CloudHanle& SteamManager::getCloud()
{
	return m_cloudH;
}


#pragma endregion

#pragma region MANETTE

ManetteHandle::ManetteHandle() : m_nbManette(0), m_actionSet(0u), m_rebindController(false)
{
	for (int i = 0; i < STEAM_INPUT_MAX_COUNT; i++)
		m_manetteHandles[i] = 0u;
}

ManetteHandle::~ManetteHandle()
{
	m_buttonsActions.clear();
	m_analogActions.clear();
	m_actions.clear();
}

void ManetteHandle::init(std::string bindGroupName)
{
	m_nbManette = SteamInput()->GetConnectedControllers(m_manetteHandles);
	m_actionSet = SteamInput()->GetActionSetHandle(bindGroupName.c_str());
	SteamInput()->ActivateActionSet(m_manetteHandles[0], m_actionSet);
	m_bindGroupName = bindGroupName;
	m_rebindController = false;
	
}

void ManetteHandle::update()
{
	if (!SteamInput()->GetConnectedControllers(m_manetteHandles))
		m_rebindController = true;

	if (m_rebindController)
	{
		m_nbManette = SteamInput()->GetConnectedControllers(this->getHandleController());
		m_actionSet = SteamInput()->GetActionSetHandle(m_bindGroupName.c_str());
		SteamInput()->ActivateActionSet(m_manetteHandles[0], m_actionSet);

		std::map<std::string, InputDigitalActionData_t> tmp_bindButtons;
		for (auto& button : m_buttonsActions)
		{
			InputDigitalActionHandle_t tmp_buttonAction = SteamInput()->GetDigitalActionHandle(button.first.c_str());
			tmp_bindButtons[button.first] = SteamInput()->GetDigitalActionData(m_manetteHandles[0], tmp_buttonAction);
		}

		m_buttonsActions.clear();
		m_buttonsActions = tmp_bindButtons;

		std::map<std::string, std::tuple<InputAnalogActionData_t, std::array<sf::Vector2f, 10>, unsigned short>> tmp_bindAnalog;
		for (auto& analog : m_analogActions)
		{
			InputAnalogActionHandle_t tmp_analogAction = SteamInput()->GetAnalogActionHandle(analog.first.c_str());
			tmp_bindAnalog[analog.first] = std::make_tuple(SteamInput()->GetAnalogActionData(m_manetteHandles[0], tmp_analogAction), std::array<sf::Vector2f, 10>{}, 0u);
		}

		m_analogActions.clear();
		m_analogActions = tmp_bindAnalog;

		if (m_nbManette > 0)
			m_rebindController = false;
	}

	InputDigitalActionHandle_t tmp_buttonAction;
	for (auto& button : m_buttonsActions)
	{
		tmp_buttonAction = SteamInput()->GetDigitalActionHandle(button.first.c_str());
		button.second = SteamInput()->GetDigitalActionData(m_manetteHandles[0], tmp_buttonAction);
	}

	InputAnalogActionHandle_t tmp_analogAction;
	InputActionSetHandle_t tmp_actionSetHandle;
	InputAnalogActionData_t tmp_analogPosition;
	for (auto& analog : m_analogActions)
	{
		tmp_analogAction = SteamInput()->GetAnalogActionHandle(analog.first.c_str());
		tmp_analogPosition = SteamInput()->GetAnalogActionData(m_manetteHandles[0], tmp_analogAction);
		
		std::get<1>(analog.second)[std::get<2>(analog.second)] = sf::Vector2f(tmp_analogPosition.x, tmp_analogPosition.y);
		std::get<2>(analog.second)++;

		if (std::get<2>(analog.second) == static_cast<unsigned short>(std::get<1>(analog.second).size()))
		{
			if (std::find_if(std::get<1>(analog.second).begin(), std::get<1>(analog.second).end(), [](sf::Vector2f _position)
				{
					if (_position == sf::Vector2f())
						return false;
					else
						return true;
				}) == std::get<1>(analog.second).end())
			{
				std::get<0>(analog.second) = tmp_analogPosition;
			}

			std::get<2>(analog.second) = 0u;
		}

		if (tmp_analogPosition.x != 0.f || tmp_analogPosition.y != 0.f)
		{
			std::get<0>(analog.second) = tmp_analogPosition;
		}

		tmp_actionSetHandle = SteamInput()->GetActionSetHandle(analog.first.c_str());
		SteamInput()->ActivateActionSet(m_manetteHandles[0], tmp_actionSetHandle);
	}
}
void ManetteHandle::setVibration(unsigned short usLeftSpeed, unsigned short usRightSpeed) const
{
	SteamInput()->TriggerVibration(m_manetteHandles[0], usLeftSpeed, usRightSpeed);
}
void ManetteHandle::setDualSenseTriggerEffect(uint8_t l_start, uint8_t l_end, uint8_t l_strenght, uint8_t r_start, uint8_t r_end, uint8_t r_strenght)
{
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_L2].mode = SCE_PAD_TRIGGER_EFFECT_MODE_VIBRATION;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_L2].commandData.weaponParam.startPosition = l_start;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_L2].commandData.weaponParam.endPosition = l_end;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_L2].commandData.weaponParam.strength = l_strenght;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].mode = SCE_PAD_TRIGGER_EFFECT_MODE_VIBRATION;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.startPosition = l_start;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.endPosition = l_end;
	param.command[SCE_PAD_TRIGGER_EFFECT_PARAM_INDEX_FOR_R2].commandData.weaponParam.strength = l_strenght;
	SteamInput()->SetDualSenseTriggerEffect(m_manetteHandles[0], &param);
}

void ManetteHandle::create_analog_action(std::string action)
{
	InputAnalogActionHandle_t tmp_analogAction = SteamInput()->GetAnalogActionHandle(action.c_str());
	m_analogActions[action] = std::make_tuple(SteamInput()->GetAnalogActionData(m_manetteHandles[0], tmp_analogAction), std::array<sf::Vector2f, 10>{}, 0u);
}
void ManetteHandle::create_button_action(std::string action)
{
	InputDigitalActionHandle_t tmp_buttonAction = SteamInput()->GetDigitalActionHandle(action.c_str());
	m_buttonsActions[action] = SteamInput()->GetDigitalActionData(m_manetteHandles[0], tmp_buttonAction);
}
#pragma endregion

#pragma region ACHIEVMENT

AchievmentHandle::AchievmentHandle() {}

//AchievmentHandle::~AchievmentHandle() = default;

bool AchievmentHandle::initFromSteamworks()
{
	if (NULL == SteamUserStats() || NULL == SteamUser())
		return false;

	if (!SteamUser()->BLoggedOn())
		return false;

	SteamUserStats()->RequestCurrentStats();
	return true;
}

bool AchievmentHandle::setStat(const char* name, int value)
{
	return SteamUserStats()->SetStat(name, value);
}

bool AchievmentHandle::getStat(const char* name, int* value)
{
	return SteamUserStats()->GetStat(name, value);
}

bool AchievmentHandle::incrementStat(const char* name, int increment)
{
	return SteamUserStats()->SetStat(name, increment);
}

bool AchievmentHandle::storeStats()
{
	return SteamUserStats()->StoreStats();
}

bool AchievmentHandle::setAchievement(const char* name)
{
	return SteamUserStats()->SetAchievement(name);
}

bool AchievmentHandle::clearAchievement(const char* name)
{
	return SteamUserStats()->ClearAchievement(name);
}

bool AchievmentHandle::request()
{
	return SteamUserStats()->RequestCurrentStats();
}

#pragma endregion

#pragma region LOBBY

LobbyHandle::LobbyHandle() : m_connectedToLobby(false), m_numLobbies(0), m_CallbackCreateLobby(this, &LobbyHandle::OnLobbyCreated)
{

}

LobbyHandle::~LobbyHandle()
{
}

void LobbyHandle::createLobby(ELobbyType LobbyType, int MaxMembers)
{
	SteamAPICall_t hAPICall = SteamMatchmaking()->CreateLobby(LobbyType, MaxMembers);
	std::cout << hAPICall << '\n';
}

void LobbyHandle::OnLobbyCreated(LobbyCreated_t* pParam)
{
	if (pParam->m_eResult != k_EResultOK)
	{
		std::cout << "Erreur lors de la creation de la salle d'attente : " << pParam->m_eResult << '\n';
	}
	else
	{
		m_currentLobby = pParam->m_ulSteamIDLobby;
		std::cout << "Salle d'attente creer avec succes ! ID de la salle : " << m_currentLobby.ConvertToUint64() << '\n';
	}
}


void LobbyHandle::searchLobby()
{
	SteamAPICall_t hAPICallSteam = SteamMatchmaking()->RequestLobbyList();
	m_CallbackLobbyDataUpdated.Set(hAPICallSteam, this, &LobbyHandle::OnLobbyDataUpdated);
}

void LobbyHandle::inviteFriendtoLobby(CSteamID playerSteamID)
{
	//SteamFriends()->InviteUserToGame(playerSteamID);
}

void LobbyHandle::connectToLobby(CSteamID remoteSteamID)
{
	SteamMatchmaking()->JoinLobby(remoteSteamID);
	m_currentLobby = remoteSteamID;
	m_connectedToLobby = true;
}

bool LobbyHandle::connectRandomLobby()
{
	for (int i = 0; i <= m_numLobbies; i++)
	{
		CSteamID lobbyID = SteamMatchmaking()->GetLobbyByIndex(i);
		if (SteamMatchmaking()->GetNumLobbyMembers(lobbyID) < 2)
		{
			m_currentLobby = lobbyID;
			m_connectedToLobby = true;
			std::cout << "------- connect to lobby " << m_currentLobby.ConvertToUint64() << " ------- \n";
			connectToLobby(lobbyID);
			return true;
		}
		else
		{
			std::cout << "------- no lobby ------- \n";
			return false;
		}
	}

	return false;
}

void LobbyHandle::disconnectLobby()
{
	SteamMatchmaking()->LeaveLobby(m_currentLobby);
	m_currentLobby = k_steamIDNil;
	m_connectedToLobby = false;
}

bool LobbyHandle::isConnectedToLobby()
{
	return m_connectedToLobby;
}

int LobbyHandle::getNumLobbies()
{
	std::cout << "------- nb lobby : " << m_numLobbies << " ------- \n";
	return m_numLobbies;
}

CSteamID LobbyHandle::getCureentLobby()
{
	return m_currentLobby;
}

void LobbyHandle::OnLobbyDataUpdated(LobbyMatchList_t* pCallback, bool)
{
	if (pCallback)
	{
		m_numLobbies = static_cast<int>(pCallback->m_nLobbiesMatching);
	}
}

#pragma endregion

#pragma region Cloud

CloudHanle::CloudHanle()
{
    if (!SteamRemoteStorage() && !SteamRemoteStorage()->IsCloudEnabledForApp())
        std::cerr << "Erreur lors de l'initialisation du Cloud Steam." << "\n";
    else
    std::cerr << "Init du Cloud Steam succesfull" << "\n";
}

bool CloudHanle::saveDataToCloud(const std::string& filename, const void* data, int dataSize)
{
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return false;          // Cloud Steam non disponible ou desactive

    return SteamRemoteStorage()->FileWrite(filename.c_str(), data, dataSize);
}

int CloudHanle::loadDataFromCloud(const std::string& filename, void* buffer, int bufferSize)
{
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return -1;         // Cloud Steam non disponible ou desactive

    return SteamRemoteStorage()->FileRead(filename.c_str(), buffer, bufferSize);
}

bool CloudHanle::isCloudEnabled()
{
    return SteamRemoteStorage()->IsCloudEnabledForApp();
}

bool CloudHanle::isCloudFileExists(const std::string& filename)
{
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return false; // Cloud Steam non disponible ou desactive

    return SteamRemoteStorage()->FileExists(filename.c_str());
}

bool CloudHanle::deleteCloudFile(const std::string& filename)
{
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return false; // Cloud Steam non disponible ou desactive

    return SteamRemoteStorage()->FileDelete(filename.c_str());
}

std::vector<std::pair<std::string, int32>> CloudHanle::listCloudFiles()
{
    std::vector<std::pair<std::string, int32>> fileList;
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return fileList;

    for (int i = 0; i < SteamRemoteStorage()->GetFileCount(); ++i)
    {
        int32 fileSize;
        const char *fileName = SteamRemoteStorage()->GetFileNameAndSize(i, &fileSize);
        fileList.push_back(std::make_pair(fileName, fileSize));
    }
    return fileList;
}

#pragma endregion