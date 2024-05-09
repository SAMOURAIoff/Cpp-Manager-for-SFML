#include <SFML/Network/Http.hpp>

#include "analytics.h"

void AnalyticsManager::Initialize(const char* gameKey, const char* secretKey)
{
    gameanalytics::GameAnalytics::configureAvailableResourceItemTypes("item");
	gameanalytics::GameAnalytics::configureAvailableResourceItemTypes("gems");
    gameanalytics::GameAnalytics::initialize(gameKey, secretKey);
}

void AnalyticsManager::SendEvent(const char* eventName, const char* itemType, const char* itemId, int amount) {
	gameanalytics::GameAnalytics::addResourceEvent(GAResourceFlowType::sink, itemType, itemId, amount, "USD");
}

void AnalyticsManager::AddEvent(const std::string& eventName, const std::string& eventCategory, const std::string& eventAction, const std::string& eventLabel, int eventValue) {
	gameanalytics::GameAnalytics::addBusinessEvent(eventName.c_str(), eventCategory.c_str(), eventAction.c_str(), eventLabel.c_str(), eventValue, "USD");
}

void AnalyticsManager::RemoveEvent(const std::string& eventName, const std::string& eventCategory, const std::string& eventAction, const std::string& eventLabel) {
	gameanalytics::GameAnalytics::addBusinessEvent(eventName.c_str(), eventCategory.c_str(), eventAction.c_str(), eventLabel.c_str(), 0, "USD");
}
