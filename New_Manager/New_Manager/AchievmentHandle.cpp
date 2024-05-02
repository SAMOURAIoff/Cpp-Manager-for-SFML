#include "AchievmentHandle.h"


AchievmentHandle::AchievmentHandle() {}

AchievmentHandle::~AchievmentHandle() {}

bool AchievmentHandle::initFromSteamworks()
 {
    if (NULL == SteamUserStats() || NULL == SteamUser())
        return false;

    // Vérifie si l'utilisateur est connecté à Steam
    if (!SteamUser()->BLoggedOn())
        return false;

    // Initialise les statistiques et les succès à partir de Steamworks
    SteamUserStats()->RequestCurrentStats();
    return true;
}

bool AchievmentHandle::setStat(const char* name, int value)
{
    // Définit la valeur d'une statistique
    return SteamUserStats()->SetStat(name, value);
}

bool AchievmentHandle::getStat(const char* name, int* value)
{
    // Récupère la valeur d'une statistique
    return SteamUserStats()->GetStat(name, value);
}

bool AchievmentHandle::incrementStat(const char* name, int increment)
{
    // Incrémente la valeur d'une statistique
    return SteamUserStats()->SetStat(name, increment);
}

bool AchievmentHandle::decrementStat(const char* name, int decrement)
{
    // Décrémente la valeur d'une statistique
    return SteamUserStats()->SetStat(name, decrement);
}

bool AchievmentHandle::storeStats()
{
    // Enregistre les statistiques sur le serveur Steam
    return SteamUserStats()->StoreStats();
}

bool AchievmentHandle::setAchievement(const char* name)
{
    // Définit la valeur d'un succès à true
    return SteamUserStats()->SetAchievement(name);
}

bool AchievmentHandle::clearAchievement(const char* name)
{
    // Réinitialise la valeur d'un succès à false
    return SteamUserStats()->ClearAchievement(name);
}

bool AchievmentHandle::request()
{
    return SteamUserStats()->RequestCurrentStats();
}
