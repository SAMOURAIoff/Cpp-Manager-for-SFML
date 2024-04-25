#include "AnalyticsManager.h"

AnalyticsManager::AnalyticsManager()
{
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    m_http.setHost("www.google-analytics.com");
    m_request.setMethod(sf::Http::Request::Post);
    m_request.setUri("/collect");
    m_request.setHttpVersion(1, 1); // HTTP 1.1
    m_request.setField("Content-Type", "application/x-www-form-urlencoded");
}

void AnalyticsManager::Initialize(const std::string& trackingId)
{
	m_trackingId = trackingId;
}

bool AnalyticsManager::SendEvent(std::string clientID, const std::string& category, const std::string& action, const std::string& label, int value)
{
    std::string eventData = "v=1&tid=" + m_trackingId + "&cid=" + clientID + "&t=event&ec=" + category + "&ea=" + action;
    if (!label.empty()) {
        eventData += "&el=" + label;
    }
    if (value != 0) {
        eventData += "&ev=" + std::to_string(value);
    }
    return SendHttpPostRequest("https://www.google-analytics.com/collect", eventData);
}

AnalyticsManager::~AnalyticsManager()
{
    EVP_cleanup();
}

bool AnalyticsManager::SendHttpPostRequest(const std::string& url, const std::string& data)
{
    m_request.setBody(data);
    sf::Http::Response response = m_http.sendRequest(m_request);
    if (response.getStatus() == sf::Http::Response::Ok) {
        std::cout << "Request succeeded: " << response.getBody() << std::endl;
        return true;
    }
    else {
        std::cerr << "Request failed, status code: " << response.getStatus() << std::endl;
        return false;
    }
}
