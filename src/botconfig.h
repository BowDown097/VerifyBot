#ifndef BOTCONFIG_H
#define BOTCONFIG_H
#include <string>

class BotConfig
{
public:
    static BotConfig& instance() { static BotConfig bc; return bc; }
    void initialize();

    std::string botToken() const { return m_botToken; }
    std::string steamApiKey() const { return m_steamApiKey; }
    std::string userToken() const { return m_userToken; }
private:
    std::string m_botToken;
    std::string m_steamApiKey;
    std::string m_userToken;
};

#endif // BOTCONFIG_H
