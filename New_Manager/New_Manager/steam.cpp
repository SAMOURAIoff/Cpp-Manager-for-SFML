#include "steam.h"


void OnLobbyListUpdated(const LobbyMatchList_t* pCallback, bool bIOFailure)
{
	ServeurHandle().OnLobbyDataUpdated(pCallback, bIOFailure);
}

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

void SteamManager::LobbyListUpdatedCallback(const LobbyMatchList_t* pCallback, bool bIOFailure)
{
	OnLobbyListUpdated(pCallback, bIOFailure);
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
	if (SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 2))
		std::cout << "------- create lobby ------- \n";
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
			connectToLobby(lobbyID);
			m_currentLobby = lobbyID;
			m_connectedToLobby = true;
			std::cout << "------- connect to lobby ------- \n";
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


