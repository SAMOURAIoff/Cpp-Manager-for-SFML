#include "steam.h"


#pragma region API

SteamManager::SteamManager()
{
	std::cout << "------- Starting steam API load ------- \n";

	if (SteamAPI_Init())
	{
		SteamInput()->Init(true);

		getServeur().searchLobby();
	}

	std::cout << "------- Steam API loading finish ------- \n\n\n";
}
void SteamManager::update()
{
	SteamAPI_RunCallbacks();
}
SteamManager::~SteamManager()
{
	SteamAPI_Shutdown();
}
ServeurHandle& SteamManager::getServeur()
{
	return m_serveurH;
}


#pragma endregion


#pragma region SERVEUR

ServeurHandle::ServeurHandle() : m_connectedToLobby(false), m_numLobbies(0)
{
}

ServeurHandle::~ServeurHandle()
{
}

void ServeurHandle::createLobby()
{
	SteamAPICall_t hAPICall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 2);

	m_CallbackCreateLobby.Register(this, &ServeurHandle::OnLobbyCreated);
}

void ServeurHandle::OnLobbyCreated( CallbackData_CreateLobby* pData)
{
	if (!pData->bIOFailure) {
		// Utilisez pData->CallbackResult pour accéder aux données de retour
		LobbyCreated_t lobbyData = pData->CallbackResult;
		// Faites quelque chose avec les données du lobby créé...
	}
	else {
		// Gérer les erreurs de création de lobby
	}
}

void ServeurHandle::searchLobby()
{
	SteamMatchmaking()->RequestLobbyList();
}

void ServeurHandle::inviteFriendtoLobby(CSteamID playerSteamID)
{
	SteamFriends()->InviteUserToGame(playerSteamID, "Rejoignez ma partie !");
}

void ServeurHandle::connectToLobby(CSteamID remoteSteamID)
{
	SteamMatchmaking()->JoinLobby(remoteSteamID);
	m_currentLobby = remoteSteamID;
	m_connectedToLobby = true;
}

void ServeurHandle::connectRandomLobby()
{
	for (int i = 0; i < m_numLobbies; ++i)
	{
		CSteamID lobbyID = SteamMatchmaking()->GetLobbyByIndex(i);
		if (SteamMatchmaking()->GetNumLobbyMembers(lobbyID) < 2)
		{
			m_currentLobby = lobbyID;
			m_connectedToLobby = true;
			std::cout << "------- connect to lobby ------- \n";
			connectToLobby(lobbyID);
			return;
		}
		else
		{
			std::cout << "------- no lobby ------- \n";
		}
	}
}

void ServeurHandle::disconnectLobby()
{
	SteamMatchmaking()->LeaveLobby(m_currentLobby);
	m_currentLobby = k_steamIDNil;
	m_connectedToLobby = false;
}

bool ServeurHandle::isConnectedToLobby()
{
	return m_connectedToLobby;
}

int ServeurHandle::getNumLobbies()
{
	std::cout << "------- nb lobby : " << m_numLobbies << " ------- \n";
	return m_numLobbies;
}

void ServeurHandle::OnLobbyDataUpdated(const LobbyMatchList_t* pCallback, bool bIOFailure)
{
	if (pCallback)
	{
		m_numLobbies = pCallback->m_nLobbiesMatching;
	}
}

#pragma endregion


