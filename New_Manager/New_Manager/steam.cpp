#include "steam.h"


#pragma region API

SteamManager::SteamManager()
{
	std::cout << "------- Starting steam API load ------- \n";

	if (SteamAPI_Init())
	{
		SteamInput()->Init(true);
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

ServeurHandle::ServeurHandle() : m_connectedToLobby(false), m_numLobbies(0), m_CallbackCreateLobby(this, &ServeurHandle::OnLobbyCreated)
{	
	
}

ServeurHandle::~ServeurHandle()
{
}

void ServeurHandle::createLobby(ELobbyType LobbyType, int MaxMembers)
{
	SteamAPICall_t hAPICall = SteamMatchmaking()->CreateLobby(LobbyType, MaxMembers);
}

void ServeurHandle::OnLobbyCreated(LobbyCreated_t* pParam)
{
	if (pParam->m_eResult != k_EResultOK)
	{
		std::cout << "Erreur lors de la cr�ation de la salle d'attente : " << pParam->m_eResult << std::endl;
	}
	else
	{
		m_currentLobby = pParam->m_ulSteamIDLobby;
		std::cout << "Salle d'attente cr��e avec succ�s ! ID de la salle : " << m_currentLobby.ConvertToUint64() << std::endl;
	}
}

void ServeurHandle::sendDataToOtherPlayers(const void* data, int dataSize)
{
	// Envoyer les donn�es de position au lobby via Steam Networking
	if (m_currentLobby.IsValid())
	{
		if (dataSize <= 0 || dataSize > k_nMaxSteamNetworkingPayloadSize)
		{
			std::cerr << "Taille de donn�es invalide." << std::endl;
		}

		// Envoi des donn�es � tous les autres joueurs du lobby
		for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(m_currentLobby); ++i)
		{
			CSteamID playerID = SteamMatchmaking()->GetLobbyMemberByIndex(m_currentLobby, i);

			// Ne pas envoyer les donn�es au joueur local
			if (playerID != SteamUser()->GetSteamID())
			{
				// Envoyer les donn�es � chaque joueur du lobby
				if (!SteamNetworking()->SendP2PPacket(playerID, data, dataSize, k_EP2PSendReliable, CHANNEL_GLOBAL_DATA))
				{
					std::cerr << "�chec de l'envoi des donn�es globales au joueur " << playerID.ConvertToUint64() << std::endl;
				}
			}
		}
	}
}

void ServeurHandle::receiveDataFromOtherPlayers(void* buffer, int bufferSize)
{
	// Recevoir les donn�es de position des autres joueurs du lobby
	if (m_currentLobby.IsValid())
	{
		if (!buffer || bufferSize <= 0 || bufferSize > k_nMaxSteamNetworkingPayloadSize)
		{
			std::cerr << "Tampon de r�ception invalide." << std::endl;
		}

		while (SteamNetworking()->IsP2PPacketAvailable(NULL))
		{
			uint32 packetSize = 0;
			CSteamID senderID;

			// Lire le paquet
			if (SteamNetworking()->ReadP2PPacket(buffer, bufferSize, &packetSize, &senderID, NULL))
			{
				std::cout << "Donn�es re�ues de joueur " << senderID.ConvertToUint64() << std::endl;
			}
			else
			{
				std::cerr << "Erreur lors de la lecture des donn�es globales." << std::endl;
			}
		}

	}
}

void ServeurHandle::searchLobby()
{
	SteamAPICall_t hAPICallSteam = SteamMatchmaking()->RequestLobbyList();
	m_CallbackLobbyDataUpdated.Set(hAPICallSteam, this, &ServeurHandle::OnLobbyDataUpdated);
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

bool ServeurHandle::connectRandomLobby()
{
	for (int i = 0; i < m_numLobbies; ++i)
	{
		CSteamID lobbyID = SteamMatchmaking()->GetLobbyByIndex(i);
		if (SteamMatchmaking()->GetNumLobbyMembers(lobbyID) < 2)
		{
			m_currentLobby = lobbyID;
			m_connectedToLobby = true;
			std::cout << "------- connect to lobby "<< m_currentLobby.ConvertToUint64() <<" ------- \n";
			connectToLobby(lobbyID);
			return true;
		}
		else
		{
			std::cout << "------- no lobby ------- \n";
			return false;
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

CSteamID ServeurHandle::getCureentLobby()
{
	return m_currentLobby;
}

void ServeurHandle::OnLobbyDataUpdated(LobbyMatchList_t* pCallback,bool)
{
	if (pCallback)
	{
		m_numLobbies = pCallback->m_nLobbiesMatching;
	}
}

#pragma endregion

