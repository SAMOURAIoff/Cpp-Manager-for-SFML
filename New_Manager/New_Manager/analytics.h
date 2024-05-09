#include <SFML/Network.hpp>
#include <iostream>


class AnalyticsManager
{
public:
    AnalyticsManager();

    void Initialize(const std::string& trackingId);

    bool SendEvent(std::string clientID, const std::string& category, const std::string& action, const std::string& label = "", int value = 0);

    ~AnalyticsManager();

private:
    std::string m_trackingId;
    sf::Http m_http;
    sf::Http::Request m_request;

    bool SendHttpPostRequest(const std::string& url, const std::string& data);
};
