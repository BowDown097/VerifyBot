#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>

namespace Constants
{
    constexpr auto FAQA1TPL = "Follow the steps in {} and you'll get access to the #mod-download, #help-requests, and #mod-making "
                              "channels (hopefully self-explanatory). Currently, **only Windows and Linux are supported.**";
    constexpr auto FAQA2TPL = "Mods are made in C# with [BTD Mod Helper](https://github.com/gurrenm3/BTD-Mod-Helper). "
                              "If you've finished a mod, post it in #mod-making and ping a staff member, and if it abides by the "
                              "[mod submission guidelines](https://pastebin.com/T8T8EJQ5), we'll put it up in #mod-download.";
    constexpr auto FAQA3TPL = "As far as mods in this server are concerned, you should be safe as long as you stay out of the online "
                              "modes. Otherwise, you'll be put in the hacker pool, which will bar you from several online activities, "
                              "even if you have mods disabled. You'll know you're hacker pooled if there are two leaves above the "
                              "settings icon in the main menu.";
    constexpr auto FAQA4TPL = "See [this message](https://discord.com/channels/504782676331331584/504807965597892618/793865515910168637) for more details.";
    constexpr auto FAQA5TPL = "You need to have a sizeable influence in the Bloons scene. With the role, you get access to mod betas "
                              "and your uploads will be put in {}.";
    constexpr auto FAQA6TPL = "You need a decent assortment of mods and have thorough knowledge of modding the game.";

    constexpr auto BTD6EpicId = "6a8dfa6e441e4f2f9048a98776c6077d";
    constexpr uint32_t BTD6SteamId = 960090;
    constexpr uint64_t VerifyChanId = 715598300505702440;
    constexpr uint64_t VerifyInstructMsgId = 1009150994556399677;
    constexpr uint64_t VerifiedEpicRoleId = 1134543406387961886;
    constexpr uint64_t VerifiedSteamRoleId = 511848372999553035;
    constexpr uint64_t YTAnnounceChanId = 787834326917316628;

    constexpr auto VerifyDAPIFailTPL = "Failed to get profile info. Status code: {} Reason: {}";
    constexpr auto VerifyNoBTD6EpicTPL = "I could not find BTD6 in your Epic Games Store games list. Make sure the account you're "
                                         "using has the game, then try again.";
    constexpr auto VerifyNoBTD6SteamTPL = "I could not find BTD6 in your Steam games list. Do you perhaps not have your games "
                                          "public, or not have BTD6? Make sure you do, then try again. If you do not know how to "
                                          "make your account/games public, look it up.";
    constexpr auto VerifyNoSteamTPL = "I could not find Steam in your connected accounts. Do you perhaps not have it "
                                      "connected? Make sure you do in Discord Settings -> Connections, then try again.";
    constexpr auto VerifySAPIFailTPL = "Steam API request failed. Status code: {} Reason: {}";
    constexpr auto VerifyWrongChannelTPL = "Please execute this command in {}.";
}

#endif // CONSTANTS_H
