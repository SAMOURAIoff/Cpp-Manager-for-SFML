#pragma once
#include "SFML_ENGINE/Tools.h"

void OnLobbyListUpdated(const LobbyMatchList_t* pCallback, bool bIOFailure);


class ServeurHandle
{
private:
	bool m_connectedToLobby;
	std::vector<CSteamID> m_joueurPret;
	CSteamID m_currentLobby;
	int m_numLobbies;

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

	void OnLobbyDataUpdated(const LobbyMatchList_t* pCallback, bool bIOFailure);
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

	static void LobbyListUpdatedCallback(const LobbyMatchList_t* pCallback, bool bIOFailure);

};