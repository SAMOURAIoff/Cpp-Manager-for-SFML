#include "CloudHandle.h"

CloudManager::CloudManager()
{
    if (!initCloud())
        std::cerr << "Erreur lors de l'initialisation du Cloud Steam." << std::endl;
    else
    std::cerr << "Init du Cloud Steam succesfull" << std::endl;
}

bool CloudManager::initCloud()
{
    return SteamRemoteStorage() && SteamRemoteStorage()->IsCloudEnabledForApp();
}

bool CloudManager::saveDataToCloud(const std::string& filename, const void* data, int dataSize)
{
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return false;          // Cloud Steam non disponible ou desactive

    return SteamRemoteStorage()->FileWrite(filename.c_str(), data, dataSize);
}

int CloudManager::loadDataFromCloud(const std::string& filename, void* buffer, int bufferSize)
{
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return -1;         // Cloud Steam non disponible ou desactive

    return SteamRemoteStorage()->FileRead(filename.c_str(), buffer, bufferSize);
}

bool CloudManager::isCloudEnabled()
{
    return SteamRemoteStorage()->IsCloudEnabledForApp();
}

bool CloudManager::isCloudFileExists(const std::string& filename)
{
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return false; // Cloud Steam non disponible ou desactive

    return SteamRemoteStorage()->FileExists(filename.c_str());
}

bool CloudManager::deleteCloudFile(const std::string& filename)
{
    if (!SteamRemoteStorage() || !isCloudEnabled())
        return false; // Cloud Steam non disponible ou desactive

    return SteamRemoteStorage()->FileDelete(filename.c_str());
}

std::vector<std::pair<std::string, int32>> CloudManager::listCloudFiles()
{
    std::vector<std::pair<std::string, int32>> fileList;
    

    if (!SteamRemoteStorage() || !isCloudEnabled())
        return fileList;

    for (int i = 0; i < SteamRemoteStorage()->GetFileCount(); ++i) {
        int32 fileSize;
        const char *fileName = SteamRemoteStorage()->GetFileNameAndSize(i, &fileSize);
        fileList.push_back(std::make_pair(fileName, fileSize));
    }

    return fileList;
}
