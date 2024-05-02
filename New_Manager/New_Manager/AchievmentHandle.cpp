#include "AchievmentHandle.h"


AchievmentHandle::AchievmentHandle() {}

AchievmentHandle::~AchievmentHandle() {}

bool AchievmentHandle::initFromSteamworks()
 {
    if (NULL == SteamUserStats() || NULL == SteamUser())
        return false;

    // V�rifie si l'utilisateur est connect� � Steam
    if (!SteamUser()->BLoggedOn())
        return false;

    // Initialise les statistiques et les succ�s � partir de Steamworks
    SteamUserStats()->RequestCurrentStats();
    return true;
}

bool AchievmentHandle::setStat(const char* name, int value)
{
    // D�finit la valeur d'une statistique
    return SteamUserStats()->SetStat(name, value);
}

bool AchievmentHandle::getStat(const char* name, int* value)
{
    // R�cup�re la valeur d'une statistique
    return SteamUserStats()->GetStat(name, value);
}

bool AchievmentHandle::incrementStat(const char* name, int increment)
{
    // Incr�mente la valeur d'une statistique
    return SteamUserStats()->SetStat(name, increment);
}

bool AchievmentHandle::decrementStat(const char* name, int decrement)
{
    // D�cr�mente la valeur d'une statistique
    return SteamUserStats()->SetStat(name, decrement);
}

bool AchievmentHandle::storeStats()
{
    // Enregistre les statistiques sur le serveur Steam
    return SteamUserStats()->StoreStats();
}

bool AchievmentHandle::setAchievement(const char* name)
{
    // D�finit la valeur d'un succ�s � true
    return SteamUserStats()->SetAchievement(name);
}

bool AchievmentHandle::clearAchievement(const char* name)
{
    // R�initialise la valeur d'un succ�s � false
    return SteamUserStats()->ClearAchievement(name);
}

bool AchievmentHandle::request()
{
    return SteamUserStats()->RequestCurrentStats();
}
