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
		std::cout << "Erreur lors de la création de la salle d'attente : " << pParam->m_eResult << std::endl;
	}
	else
	{
		m_currentLobby = pParam->m_ulSteamIDLobby;
		std::cout << "Salle d'attente créée avec succès ! ID de la salle : " << m_currentLobby.ConvertToUint64() << std::endl;
	}
}

void ServeurHandle::sendDataToOtherPlayers(const void* data, int dataSize)
{
	if (m_currentLobby.IsValid())
	{
		if (dataSize <= 0 || dataSize > k_nMaxSteamNetworkingPayloadSize)
		{
			std::cerr << "Taille de donnes invalide." << std::endl;
			return;
		}

		for (int i = 0; i < SteamMatchmaking()->GetNumLobbyMembers(m_currentLobby); ++i)
		{
			CSteamID playerID = SteamMatchmaking()->GetLobbyMemberByIndex(m_currentLobby, i);

			if (playerID != SteamUser()->GetSteamID())
			{
				if (SteamNetworking()->SendP2PPacket(playerID, data, dataSize, k_EP2PSendReliable, CHANNEL_GLOBAL_DATA))
				{
					std::cerr << "envoi des données "<< data <<" au joueur " << playerID.ConvertToUint64() << std::endl;
				}
				else
				{
					std::cerr << "Échec de l'envoi des données globales au joueur " << playerID.ConvertToUint64() << std::endl;
					return;
				}
			}
		}
	}
}

void ServeurHandle::receiveDataFromOtherPlayers(void* buffer, int bufferSize)
{
	if (m_currentLobby.IsValid())
	{
		if (!buffer || bufferSize <= 0 || bufferSize > k_nMaxSteamNetworkingPayloadSize)
		{
			std::cerr << "Tampon de reception invalide." << std::endl;
			return;
		}

		while (SteamNetworking()->IsP2PPacketAvailable(NULL))
		{
			uint32 packetSize = 0;
			CSteamID senderID;

			if (SteamNetworking()->ReadP2PPacket(buffer, bufferSize, &packetSize, &senderID, NULL))
			{
				if (senderID == m_currentLobby)
				{
					if (packetSize <= 0 || packetSize > k_nMaxSteamNetworkingPayloadSize)
					{
						std::cerr << "Taille de paquet invalide." << std::endl;
						return;
					}
					std::cout << "Données reçues : ";
					for (int i = 0; i < packetSize; ++i)
					{
						std::cout << static_cast<char*>(buffer)[i];
					}
					std::cout << std::endl;


					//std::cout << "Données reçues" << buffer << "de joueur " << senderID.ConvertToUint64() << std::endl;
				}
				else
				{
					std::cerr << "Données reçues d'un lobby non autorisé." << std::endl;
					return;
				}
			}
			else
			{
				std::cerr << "Erreur lors de la lecture des donnes globales." << std::endl;
				return;
			}
		}
	}
}

void ServeurHandle::connectP2PWithPlayersInLobby()
{
	if (m_currentLobby.IsValid())
	{
		int numMembers = SteamMatchmaking()->GetNumLobbyMembers(m_currentLobby);
		for (int i = 0; i < numMembers; ++i)
		{
			CSteamID memberID = SteamMatchmaking()->GetLobbyMemberByIndex(m_currentLobby, i);
			if (memberID != SteamUser()->GetSteamID()) // Ne pas se connecter à soi-même
			{
				SteamNetworking()->AcceptP2PSessionWithUser(memberID);
			}
		}
	}
	else
	{
		std::cerr << "Aucun lobby valide n'est actuellement connecté." << std::endl;
	}
}


bool ServeurHandle::isPacketAvailable()
{
	return SteamNetworking()->IsP2PPacketAvailable(NULL);
}

void ServeurHandle::checkP2PConnectionState(CSteamID remoteSteamID)
{
	P2PSessionState_t p2pSessionState;
	if (SteamNetworking()->GetP2PSessionState(remoteSteamID, &p2pSessionState))
	{
		std::cout << "État de la connexion P2P avec joueur " << remoteSteamID.ConvertToUint64() << ":" << std::endl;
		std::cout << " - Connexion établie : " << (p2pSessionState.m_bConnectionActive ? "Oui" : "Non") << std::endl;
		std::cout << " - En cours de connexion : " << (p2pSessionState.m_bConnecting ? "Oui" : "Non") << std::endl;
		std::cout << " - Erreur de session P2P : " << static_cast<int>(p2pSessionState.m_eP2PSessionError) << std::endl;
		std::cout << " - Utilisation du relais : " << (p2pSessionState.m_bUsingRelay ? "Oui" : "Non") << std::endl;
		std::cout << " - Octets en attente d'envoi : " << p2pSessionState.m_nBytesQueuedForSend << std::endl;
		std::cout << " - Paquets en attente d'envoi : " << p2pSessionState.m_nPacketsQueuedForSend << std::endl;
		std::cout << " - Adresse IP distante : " << p2pSessionState.m_nRemoteIP << std::endl;
		std::cout << " - Port distant : " << p2pSessionState.m_nRemotePort << std::endl;
	}
	else
	{
		std::cerr << "Impossible d'obtenir l'état de la connexion P2P avec joueur " << remoteSteamID.ConvertToUint64() << std::endl;
	}
}

void ServeurHandle::checkAllP2PConnections()
{
	if (m_currentLobby.IsValid())
	{
		m_numMembers = SteamMatchmaking()->GetNumLobbyMembers(m_currentLobby);
		for (int i = 0; i < m_numMembers; ++i)
		{
			CSteamID memberID = SteamMatchmaking()->GetLobbyMemberByIndex(m_currentLobby, i);
			
			checkP2PConnectionState(memberID);
		}
	}
	else
	{
		std::cerr << "Aucun lobby valide n'est actuellement connecté." << std::endl;
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

