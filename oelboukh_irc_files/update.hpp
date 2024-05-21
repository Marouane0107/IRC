#ifndef UPDATE_HPP
#define UPDATE_HPP
#include "../client.hpp"
#include <iostream>
#include <string>
void update_nick_command(std::string new_nick, std::string old_nick);
void update_user_command(std::string new_user, std::string old_user);
void update_real_name_command(std::string real_name, std::string nick);
#endif