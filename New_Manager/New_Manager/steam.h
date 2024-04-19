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

	struct CallbackData_CreateLobby
	{
		bool bIOFailure;
		LobbyCreated_t CallbackResult;

		CallbackData_CreateLobby() : bIOFailure(false), CallbackResult(LobbyCreated_t()) {};
	};

	CCallback<ServeurHandle, ServeurHandle::CallbackData_CreateLobby> m_CallbackCreateLobby;
	typedef void (ServeurHandle::* CallbackFunc_t)(CallbackData_CreateLobby*);

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
	void OnLobbyCreated(CallbackData_CreateLobby* pData);
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

