#ifndef VERIFICATION_H
#define VERIFICATION_H
#include <dpp/cluster.h>

namespace Verification
{
    void doEpicGames(dpp::cluster& client, const dpp::form_submit_t& event, const std::string& authCode);
    void doSteam(dpp::cluster& client, const dpp::slashcommand_t& event);
}

#endif // VERIFICATION_H
