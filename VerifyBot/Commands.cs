namespace VerifyBot;
public class Commands : ModuleBase<SocketCommandContext>
{
    [Command("verify")]
    public async Task<RuntimeResult> Verify()
    {
        if (Context.Channel.Id != Constants.VerifyChannel)
            return CommandResult.FromError($"{Context.User.Mention}, please execute this command in {MentionUtils.MentionChannel(Constants.VerifyChannel)}.");

        try
        {
            using HttpClient client = new();
            client.DefaultRequestHeaders.Add("Authorization", Credentials.UserToken);
            string response = await client.GetStringAsync($"https://discordapp.com/api/v6/users/{Context.User.Id}/profile");
            JObject responseJson = JObject.Parse(response);
            JArray connectedAccounts = JArray.Parse(responseJson["connected_accounts"].ToString());

            if (connectedAccounts.Children().All(x => x["type"].ToString() != "steam"))
                return CommandResult.FromError($"{Context.User.Mention}, I could not find Steam in your connected accounts. Do you perhaps not have it connected? Make sure you do in Settings -> Connections, then try again.");

            foreach (JObject item in connectedAccounts.Children().Where(x => x["type"].ToString() == "steam").Cast<JObject>())
            {
                SteamWebInterfaceFactory webInterfaceFactory = new(Credentials.SteamApiKey);
                PlayerService player = webInterfaceFactory.CreateSteamWebInterface<PlayerService>();
                ulong id = item["id"].ToObject<ulong>();
                ISteamWebResponse<OwnedGamesResultModel> ownedGames = await player.GetOwnedGamesAsync(id);
                IReadOnlyCollection<OwnedGameModel> ownedGamesData = ownedGames.Data.OwnedGames;

                if (ownedGamesData.All(x => x.AppId != Constants.Btd6Id))
                    return CommandResult.FromError($"{Context.User.Mention}, I could not find BTD6 in your Steam games list. Do you perhaps not have your account/games public, or not have BTD6? Make sure you do, then try again. If you do not know how to make your account/games public, look it up.");

                await ReplyAsync($"{Context.User.Mention}, you were verified successfully and received the Purchased BTD6 on Steam role!");
                await Task.Delay(1500);
                await (Context.User as IGuildUser)?.AddRoleAsync(Context.Guild.GetRole(Constants.VerifiedRole));

                IEnumerable<IMessage> msgs = await Context.Channel.GetMessagesAsync().FlattenAsync();
                await (Context.Channel as SocketTextChannel)?.DeleteMessagesAsync(msgs.Where(m => m.Id != Constants.VerifyInstructionsMsg));
            }
        }
        catch (Exception e)
        {
            EmbedBuilder embed = new EmbedBuilder()
                .WithTitle("Shit Code Exception")
                .WithColor(Color.Red)
                .WithDescription($"{e.Message}\n{Format.Sanitize(e.StackTrace)}");
            await ReplyAsync($"{Context.User.Mention}, there seems to have been a problem verifying you. Here's the details. This problem will probably be fixed by just trying again, but if it persists, ping the server owner.",
                embed: embed.Build());
        }

        await Context.Message.DeleteAsync();
        return CommandResult.FromSuccess();
    }
}