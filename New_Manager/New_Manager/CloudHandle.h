#pragma once
#include <SFML_ENGINE/Tools.h>

class CloudManager
{
public:
    CloudManager();
    ~CloudManager() = default;
    
    // Initialise le Steam Cloud
    bool initCloud();

    // Enregistre des donnees dans le Cloud Steam
    bool saveDataToCloud(const std::string& filename, const void* data, int dataSize) ;

    // Charge des donnees depuis le Cloud Steam
    int loadDataFromCloud(const std::string& filename, void* buffer, int bufferSize);

    // Verifie si le Cloud Steam est active pour le jeu
    bool isCloudEnabled();

    // Verifie si un fichier specifique existe dans le Cloud Steam
    bool isCloudFileExists(const std::string& filename);

    // Supprime un fichier du Cloud Steam
    bool deleteCloudFile(const std::string& filename);

    std::vector<std::pair<std::string, int32>> listCloudFiles();
    
};



