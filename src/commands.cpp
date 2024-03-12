#include "commands.h"
#include "constants.h"
#include "format_polyfill.h"
#include "verification.h"
#include <cpr/cpr.h>
#include <dpp/dpp.h>

namespace Commands
{
    void makeFaq(dpp::cluster& client, const dpp::slashcommand_t& event)
    {
        dpp::command_value parameter = event.get_parameter("channel");
        if (parameter.index() == 0)
        {
            event.reply("Need a channel.");
            return;
        }

        dpp::channel c = client.channel_get_sync(std::get<dpp::snowflake>(parameter));
        if (!c.is_text_channel())
        {
            event.reply("Channel must be a text channel.");
            return;
        }

        dpp::embed embed = dpp::embed()
           .set_color(dpp::colors::blue)
           .set_title("FAQ")
           .add_field("How do I get the mods?", std::format(Constants::FAQA1TPL, dpp::channel::get_mention(Constants::VerifyChanId)))
           .add_field("How do I make mods?", Constants::FAQA2TPL)
           .add_field("Will I get banned for using mods?", Constants::FAQA3TPL)
           .add_field("How can I test upcoming mods?", Constants::FAQA4TPL)
           .add_field("How do I get the YouTubers role?", std::format(Constants::FAQA5TPL, dpp::channel::get_mention(Constants::YTAnnounceChanId)))
           .add_field("How do I get the Modders role?", Constants::FAQA6TPL);
        client.message_create(dpp::message(c.id, embed));
    }

    void verifyEpicGames(dpp::cluster& client, const dpp::slashcommand_t& event)
    {
        if (event.command.channel_id != Constants::VerifyChanId)
        {
            event.reply(std::format(Constants::VerifyWrongChannelTPL, dpp::channel::get_mention(Constants::VerifyChanId)));
            return;
        }

        dpp::interaction_modal_response modal("verify_modal", "Epic Games Verification");
        modal.add_component(
            dpp::component()
                .set_label("Authorization code")
                .set_id("auth_code")
                .set_required(true)
                .set_type(dpp::cot_text)
                .set_placeholder("https://legendary.gl/epiclogin")
                .set_min_length(32)
                .set_max_length(32)
                .set_text_style(dpp::text_short)
        );
        event.dialog(modal);
    }

    void verifySteam(dpp::cluster& client, const dpp::slashcommand_t& event)
    {
        if (event.command.channel_id != Constants::VerifyChanId)
        {
            event.reply(std::format(Constants::VerifyWrongChannelTPL, dpp::channel::get_mention(Constants::VerifyChanId)));
            return;
        }

        Verification::doSteam(client, event);
    }
}
