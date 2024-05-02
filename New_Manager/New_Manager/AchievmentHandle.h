#pragma once
#include <SFML_ENGINE/Tools.h> // Assurez-vous d'inclure l'API SteamWorks


class AchievmentHandle
{
public:
    AchievmentHandle();
    ~AchievmentHandle();

    // Initialisation des statistiques et des succ�s depuis Steamworks
    bool initFromSteamworks();

    // D�finition et r�cup�ration de la valeur d'une statistique
    bool setStat(const char* name, int value);
    bool getStat(const char* name, int* value);

    // Incr�mentation et d�cr�mentation d'une statistique
    bool incrementStat(const char* name, int increment);
    bool decrementStat(const char* name, int decrement);

    // Mise � jour des statistiques et des succ�s
    bool storeStats();

    // D�finition de la valeur d'un succ�s
    bool setAchievement(const char* name);

    // R�initialisation d'un succ�s
    bool clearAchievement(const char* name);

    bool request();

private:
    CSteamID m_steamID;
};
