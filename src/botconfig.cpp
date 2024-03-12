#include "botconfig.h"
#include <dpp/json.h>
#include <fstream>
#include <iostream>

void BotConfig::initialize()
{
    std::ifstream stream("config.json");
    if (!stream.good())
    {
        std::cout << "No configuration file found! See GitHub page for more info." << std::endl;
        exit(EXIT_FAILURE);
    }

    try
    {
        nlohmann::json reader;
        stream >> reader;
        m_botToken = reader["botToken"].template get<std::string>();
        m_steamApiKey = reader["steamApiKey"].template get<std::string>();
        m_userToken = reader["userToken"].template get<std::string>();
    }
    catch (const nlohmann::json::exception& ex)
    {
        std::cout << "Configuration parsing failed: " << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
