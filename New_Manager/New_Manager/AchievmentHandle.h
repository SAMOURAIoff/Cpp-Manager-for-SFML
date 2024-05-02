#pragma once
#include <SFML_ENGINE/Tools.h> // Assurez-vous d'inclure l'API SteamWorks


class AchievmentHandle
{
public:
    AchievmentHandle();
    ~AchievmentHandle();

    // Initialisation des statistiques et des succès depuis Steamworks
    bool initFromSteamworks();

    // Définition et récupération de la valeur d'une statistique
    bool setStat(const char* name, int value);
    bool getStat(const char* name, int* value);

    // Incrémentation et décrémentation d'une statistique
    bool incrementStat(const char* name, int increment);
    bool decrementStat(const char* name, int decrement);

    // Mise à jour des statistiques et des succès
    bool storeStats();

    // Définition de la valeur d'un succès
    bool setAchievement(const char* name);

    // Réinitialisation d'un succès
    bool clearAchievement(const char* name);

    bool request();

private:
    CSteamID m_steamID;
};
