#include <dpp/dpp.h>
#include "botcommandhandler.h"
#include "botconfig.h"
#include "verification.h"
#include "commands.h"

int main()
{
    BotConfig::instance().initialize();
    dpp::cluster client(BotConfig::instance().botToken(), dpp::i_default_intents | dpp::i_message_content);
    client.on_log(dpp::utility::cout_logger());

    BotCommandHandler cmdHandler(client);

    client.on_ready([&client, &cmdHandler](const dpp::ready_t& event) {
        client.set_presence(dpp::presence(dpp::ps_online, dpp::at_game, "(〃´∀｀〃)ε｀●)chu♪"));
        if (dpp::run_once<struct register_bot_commands>())
        {
            dpp::slashcommand mkfaq("mkfaq", "Create FAQ in the FAQ channel.", client.me.id);
            mkfaq.add_option(dpp::command_option(dpp::command_option_type::co_channel, "channel", "Text channel to send in", true));
            mkfaq.set_default_permissions(0); // make available only to admins

            cmdHandler.addCommand(mkfaq, Commands::makeFaq);
            cmdHandler.addCommand(dpp::slashcommand("verifyepic", "Verify you own BTD6 on Epic Games.", client.me.id), Commands::verifyEpicGames);
            cmdHandler.addCommand(dpp::slashcommand("verifysteam", "Verify you own BTD6 on Steam.", client.me.id), Commands::verifySteam);
        }
    });

    client.on_form_submit([&client](const dpp::form_submit_t& event) {
        std::string authCode = std::get<std::string>(event.components[0].components[0].value);
        Verification::doEpicGames(client, event, authCode);
    });

    client.start(dpp::st_wait);
}
