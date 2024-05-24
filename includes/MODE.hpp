#ifndef mode_hpp
#define mode_hpp

#include "channel.hpp"
#include <vector>
#include <iostream>
class client_1;
class channel;
void MODE_command(std::vector<std::string> option, client_1 *user);
client_1 *search_for_client(std::string name);
void leave_channel(std::vector<std::string> tokens, client_1 *user);
#endif