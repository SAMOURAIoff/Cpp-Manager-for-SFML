#pragma once
#include "SFML_ENGINE/Tools.h"
#include <functional>

struct LobbyCreated_t;
struct LobbyMatchList_t;

enum class ECallbackType
{
	LobbyCreated,
	LobbyJoined,
};

constexpr int CHANNEL_GLOBAL_DATA = 1;
constexpr int k_nMaxSteamNetworkingPayloadSize = 1024;

class ServeurHandle
{
private:
	bool m_connectedToLobby;
	std::vector<CSteamID> m_joueurPret;
	CSteamID m_currentLobby;
	int m_numLobbies;
	int m_numMembers;

	CCallResult< ServeurHandle, LobbyMatchList_t> m_CallbackLobbyDataUpdated;
	typedef void (ServeurHandle::* LobbyDataCallback_t)(LobbyMatchList_t*, bool);

	CCallback<ServeurHandle, LobbyCreated_t> m_CallbackCreateLobby;
	typedef void (ServeurHandle::* CallbackFunc_t)(LobbyCreated_t*,bool);
	
public: 
	ServeurHandle();
	~ServeurHandle();

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

	void sendDataToOtherPlayers(const void* data, int dataSize);
	void receiveDataFromOtherPlayers(void* buffer, int bufferSize);
	bool isPacketAvailable();

	void connectP2PWithPlayersInLobby();

	void checkP2PConnectionState(CSteamID remoteSteamID);
	void checkAllP2PConnections();

};


class SteamManager
{	
private:
	ServeurHandle m_serveurH;

public:
	SteamManager();
	void update();
	~SteamManager();


	//focntion pour ouvrir l'overlay et envoyer une invite a un lobby (recup id csteam)

	ServeurHandle& getServeur();

};

