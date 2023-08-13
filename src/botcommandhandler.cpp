#include "botcommandhandler.h"

BotCommandHandler::BotCommandHandler(dpp::cluster& cluster) : cluster(cluster)
{
    cluster.on_slashcommand(std::bind(&BotCommandHandler::handOver, this, std::placeholders::_1));
}

void BotCommandHandler::addCommand(const dpp::slashcommand& cmd, commandFunc func)
{
    cluster.global_command_create(cmd);
    commands.insert({cmd.name, func});
}

void BotCommandHandler::handOver(const dpp::slashcommand_t& event)
{
    if (auto command = commands.find(event.command.get_command_name()); command != commands.end())
        command->second(cluster, event);
}
