#include <SFML/Network.hpp>
#include <iostream>
#include "gameanalytics/GameAnalytics.h"


class AnalyticsManager
{
public:
    static void Initialize(const char* gameKey, const char* secretKey);
    static void SendEvent(const char* eventName, const char* itemType, const char* itemId, int amount);
    static void AddEvent(const std::string& eventName, const std::string& eventCategory, const std::string& eventAction, const std::string& eventLabel, int eventValue);
    static void RemoveEvent(const std::string& eventName, const std::string& eventCategory, const std::string& eventAction, const std::string& eventLabel);
};
