#ifndef BOTCOMMANDHANDLER_H
#define BOTCOMMANDHANDLER_H
#include <dpp/cluster.h>
#include <functional>
#include <unordered_map>

class BotCommandHandler
{
public:
    using commandFunc = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;
    explicit BotCommandHandler(dpp::cluster& cluster);
    void addCommand(const dpp::slashcommand& cmd, commandFunc func);
private:
    dpp::cluster& cluster;
    std::unordered_map<std::string, commandFunc> commands;
    void handOver(const dpp::slashcommand_t& event);
};

#endif // BOTCOMMANDHANDLER_H
