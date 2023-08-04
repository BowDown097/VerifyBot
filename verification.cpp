#include "verification.h"
#include "botconfig.h"
#include "constants.h"
#include <cpr/cpr.h>
#include <dpp/dpp.h>

namespace Verification
{
    enum class SteamVerifyState { NoSteamConnected, NoBtd6, Success };

    void killEpicAccessToken(const std::string& accessToken, const std::string& auth)
    {
        cpr::Delete(
            cpr::Url("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/sessions/kill/" + accessToken),
            cpr::Header{{"Authorization", auth}}
        );
    }

    void verifySuccessful(dpp::cluster& client, const dpp::interaction_create_t& event, dpp::snowflake verifiedRole)
    {
        event.reply("You were verified successfully!");
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        client.guild_member_add_role(event.command.guild_id, event.command.get_issuing_user().id, verifiedRole);

        dpp::snowflake channelId = event.command.channel_id;
        client.messages_get(channelId, 0, 0, Constants::VerifyInstructMsgId, 50, [channelId, &client](const dpp::confirmation_callback_t& callback) {
            if (callback.is_error())
            {
                dpp::error_info error = callback.get_error();
                std::string errorDetails = std::format("[{}] ERROR: Error {} [{}] on getting messages to bulk delete",
                                                       dpp::utility::current_date_time(), error.code, error.message);
                std::cout << errorDetails << std::endl;
                return;
            }

            dpp::message_map messages = std::get<dpp::message_map>(callback.value);
            std::vector<dpp::snowflake> messageIds(messages.size());
            std::ranges::transform(messages, messageIds.begin(), [](const auto& pair) { return pair.first; });
            client.message_delete_bulk(messageIds, channelId);
        });
    }

    void doEpicGames(dpp::cluster& client, const dpp::form_submit_t& event, const std::string& authCode)
    {
        cpr::Response oauthResponse = cpr::Post(
            cpr::Url("https://account-public-service-prod.ol.epicgames.com/account/api/oauth/token"),
            cpr::Authentication("34a02cf8f4414e29b15921876da36f9a", "daafbccc737745039dffe53d94fc76cf", cpr::AuthMode::BASIC),
            cpr::Payload{{"grant_type", "authorization_code"}, {"code", authCode}, {"token_type", "eg1"}}
        );

        if (oauthResponse.status_code >= 500)
        {
            event.reply("Failed to start session, status code " + std::to_string(oauthResponse.status_code));
            return;
        }

        nlohmann::json oauthJson = nlohmann::json::parse(oauthResponse.text);
        if (oauthJson.contains("errorCode"))
        {
            std::string errorCode = oauthJson["errorCode"].template get<std::string>();
            if (errorCode == "errors.com.epicgames.oauth.corrective_action_required")
            {
                std::string continuationUrl = oauthJson["continuationUrl"].template get<std::string>();
                std::string correctiveAction = oauthJson["correctiveAction"].template get<std::string>();
                std::string errorMessage = oauthJson["errorMessage"].template get<std::string>();

                dpp::message msg(std::format("{} {}, open the following URL to take action: {}", errorMessage, correctiveAction, continuationUrl));
                msg.flags |= dpp::m_ephemeral;

                event.reply(msg);
                return;
            }
            else
            {
                event.reply("Failed to start session, error code " + errorCode);
                return;
            }
        }
        else if (oauthResponse.status_code >= 400)
        {
            event.reply("Failed to start session, status code " + std::to_string(oauthResponse.status_code));
            return;
        }

        std::string accessToken = oauthJson["access_token"].template get<std::string>();
        std::string accountId = oauthJson["account_id"].template get<std::string>();
        std::string auth = "Bearer " + accessToken;

        cpr::Response entitlementsResponse = cpr::Get(
            cpr::Url("https://entitlement-public-service-prod.ol.epicgames.com/entitlement/api/account/" + accountId + "/entitlements?start=0&count=5000"),
            cpr::Header{{"Authorization", auth}}
        );

        if (entitlementsResponse.status_code >= 500)
        {
            event.reply("Failed to get entitlements, status code " + std::to_string(oauthResponse.status_code));
            killEpicAccessToken(accessToken, auth);
            return;
        }

        nlohmann::json entitlementsJson = nlohmann::json::parse(entitlementsResponse.text);
        if (entitlementsJson.contains("errorCode"))
        {
            std::string errorCode = entitlementsJson["errorCode"].template get<std::string>();
            event.reply("Failed to get entitlements, error code " + errorCode);
            killEpicAccessToken(accessToken, auth);
            return;
        }
        else if (entitlementsResponse.status_code >= 400)
        {
            event.reply("Failed to get entitlements, status code " + std::to_string(oauthResponse.status_code));
            killEpicAccessToken(accessToken, auth);
            return;
        }

        bool ownsBtd6 = std::ranges::any_of(entitlementsJson, [](const nlohmann::basic_json<>& item) {
            return item["namespace"].template get<std::string>() == Constants::BTD6EpicId;
        });

        if (ownsBtd6)
            verifySuccessful(client, event, Constants::VerifiedEpicRoleId);
        else
            event.reply(Constants::VerifyNoBTD6EpicTPL);

        killEpicAccessToken(accessToken, auth);
    }

    void doSteam(dpp::cluster& client, const dpp::slashcommand_t& event)
    {
        cpr::Response response = cpr::Get(
            cpr::Url(std::format("https://discordapp.com/api/v6/users/{}/profile", (uint64_t)event.command.get_issuing_user().id)),
            cpr::Header{{"Authorization", BotConfig::instance().userToken()}}
        );

        if (response.status_code >= 400)
        {
            event.reply(std::format(Constants::VerifyDAPIFailTPL, response.status_code, response.status_line));
            return;
        }

        nlohmann::json responseJson = nlohmann::json::parse(response.text);
        SteamVerifyState verifyState = SteamVerifyState::NoSteamConnected;

        for (const nlohmann::basic_json<>& item : responseJson["connected_accounts"])
        {
            std::string type = item["type"].template get<std::string>();
            if (type != "steam")
                continue;

            verifyState = SteamVerifyState::NoBtd6;
            std::string id = item["id"].template get<std::string>();
            std::string ownedUrl = std::format(
                "https://api.steampowered.com/IPlayerService/GetOwnedGames/v1/?key={}&steamid={}",
                BotConfig::instance().steamApiKey(), id
            );

            cpr::Response ownedResponse = cpr::Get(cpr::Url(ownedUrl));
            if (ownedResponse.status_code >= 400)
            {
                event.reply(std::format(Constants::VerifySAPIFailTPL, response.status_code, response.status_line));
                return;
            }

            nlohmann::json ownedResponseJson = nlohmann::json::parse(ownedResponse.text);
            if (ownedResponseJson.empty())
                continue;

            bool ownsBtd6 = std::ranges::any_of(ownedResponseJson["response"]["games"], [](const nlohmann::basic_json<>& item) {
                return item["appid"].template get<uint32_t>() == Constants::BTD6SteamId;
            });

            if (ownsBtd6)
            {
                verifyState = SteamVerifyState::Success;
                break;
            }
        }

        switch (verifyState)
        {
        case SteamVerifyState::NoBtd6:
            event.reply(Constants::VerifyNoBTD6SteamTPL);
            break;
        case SteamVerifyState::NoSteamConnected:
            event.reply(Constants::VerifyNoSteamTPL);
            break;
        case SteamVerifyState::Success:
            verifySuccessful(client, event, Constants::VerifiedSteamRoleId);
            break;
        }
    }
}
