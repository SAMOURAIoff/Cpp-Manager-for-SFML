#pragma once
#include "SFML_ENGINE/Tools.h"
#include <functional>

struct LobbyCreated_t;
struct LobbyMatchList_t;

enum class ECallbackType
{
	LobbyCreated,
	LobbyJoined,
	// Autres types de callback nécessaires
};



class ServeurHandle
{
private:
	bool m_connectedToLobby;
	std::vector<CSteamID> m_joueurPret;
	CSteamID m_currentLobby;
	int m_numLobbies;

	CCallResult< ServeurHandle, LobbyMatchList_t> m_CallbackLobbyDataUpdated;
	typedef void (ServeurHandle::* LobbyDataCallback_t)(LobbyMatchList_t*, bool);

	CCallback<ServeurHandle, LobbyCreated_t> m_CallbackCreateLobby;
	typedef void (ServeurHandle::* CallbackFunc_t)(LobbyCreated_t*,bool);
	
public: 
	ServeurHandle();
	~ServeurHandle();

	void createLobby();
	
	void searchLobby();
	void inviteFriendtoLobby(CSteamID playerSteamID);
	void connectToLobby(CSteamID remoteSteamID);
	void connectRandomLobby();
	void disconnectLobby();
	bool isConnectedToLobby();
	int getNumLobbies();

	void OnLobbyDataUpdated(LobbyMatchList_t* pCallback, bool);
	void OnLobbyCreated(LobbyCreated_t* pParam);
};


class SteamManager
{	
private:
	ServeurHandle m_serveurH;

public:
	SteamManager();
	void update();
	~SteamManager();

	ServeurHandle& getServeur();

};

