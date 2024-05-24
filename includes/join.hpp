#ifndef MS10
#define MS10

#include <iostream>
#include <string>
#include "channel.hpp"
#include "server.hpp"

std::vector<std::string> split(const std::string& str, char delimiter);
channel *search_for_channel(std::string name);
void join_command(std::vector<std::string> tokens, client_1 *user);
void keep_recv(clien_info *cl, char *name);
void broadcast_message(channel *ch, client_1 *sender, const std::string &message);
void quit_command(channel *ch, client_1 *user);
void KICK_command(client_1 *user,std::vector<channel*> ch, std::vector<std::string> tokens);
void list_command(channel *ch);
int check_if_user_in_channel(client_1 *user, channel *ch);

#endif