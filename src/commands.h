#ifndef COMMANDS_H
#define COMMANDS_H
#include <dpp/cluster.h>

namespace Commands
{
    void makeFaq(dpp::cluster& client, const dpp::slashcommand_t& event);
    void verifyEpicGames(dpp::cluster& client, const dpp::slashcommand_t& event);
    void verifySteam(dpp::cluster& client, const dpp::slashcommand_t& event);
}

#endif // COMMANDS_H
