#include "../client.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../server.hpp"
#include "channel.hpp"
#include "Global.hpp"
#include "MODE.hpp"
#include "join.hpp"
int client_1::get_super_admin(){
	return this->super_admin;
}
void client_1::set_super_admin(int super_admin){
	this->super_admin = super_admin;
}
void update_nick_command(std::string new_nick, std::string old_nick)
{
	size_t i;
	std::cout <<"----------------- "<<  std::endl;
	std::cout <<"----------------- "<< std::endl;
	for(i = 0; i < all_clients.size(); i++)//change nick name
	{
		if(all_clients[i]->get_nick() == old_nick)
		{
			all_clients[i]->set_nick(new_nick);
			std::cout << "Nick name updated" << std::endl;
			return;
		}
	}
}

void update_user_command(std::string new_user, std::string old_user)
{
	size_t i;
	for(i = 0; i < all_clients.size(); i++)//change user name
	{
		if(all_clients[i]->get_name() == old_user)
		{
			all_clients[i]->set_name(new_user);
			std::cout << "User name updated" << std::endl;
			return;
		}
	}
}

void update_real_name_command(std::string real_name, std::string nick)
{
	size_t i;
	for(i = 0; i < all_clients.size(); i++)//change real name
	{
		if(all_clients[i]->get_nick() == nick)
		{
			all_clients[i]->set_real_name(real_name);
			std::cout << "Real name updated" << std::endl;
			return;
		}
	}
}

void check_other_commands(std::string input, client_1 *user)
{
	std::vector<std::string> tokens;
	tokens = split(input, ' ');
	if(tokens[0] == "/join" || tokens[0] == "/join\n" || tokens[0] == "join" || tokens[0] == "/JOIN" || tokens[0] == "JOIN")
	{
		// if(tokens[0] == "/join\n"){
		//     std::string msg = "Please enter the channel name after the command >/join #channel_name\n";
		//     send(user->get_socket(), msg.c_str(), msg.size(), 0);
		//     return;
		// }
		join_command(tokens, user);
	}
	else if(tokens[0] == "/list\n")//if tokens[0] == "/list" list all the users in the channel manually
	{
		if(user->ptr_channel != NULL)
			list_command(user->get_socket(),  user->ptr_channel);
		else//list all the channels in the server
		{
			if(all_channels.size() == 0)
			{
				std::string msg = "No channels in the server, try to creat one >/join #channel_name\n";
				send(user->get_socket(), msg.c_str(), msg.size(), 0);
			}
			else
			{
				size_t i;
				std::string message = "List of channels in the server:\n";
				for(i = 0; i < all_channels.size(); i++)
				{
					message += all_channels[i]->get_name() + "\n";
				}
				send(user->get_socket(), message.c_str(), message.size(), 0);
			}
		}
	}
	else if(tokens[0] == "/leave\n"){
		if(user->ptr_channel == NULL)
		{
			std::string msg = "You are not in a channel yet, try to creat one >/join #channel_name\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
		}
		else
			quit_command(user->ptr_channel, user);
	}
	 else if(tokens[0] == "/kick\n" || tokens[0] == "/kick")
	 {
		if(tokens[0] == "/kick\n"){
			std::string msg = "Please enter the user name after the command >/kick user_name\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
			return;
		}
		else
			KICK_command(user, user->ptr_channel, tokens);
	 }
	else if(tokens[0] == "/topic" || tokens[0] == "/topic\n"){
		if(user->ptr_channel == NULL){
			std::string msg = "You are not in a channel yet, try to creat one >/join #channel_name\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
			return;
		}
		if(tokens[0] == "/topic"){
			std::vector<std::string> new_topic;
			for(size_t i = 0; i < tokens.size(); i++){
				new_topic.push_back(tokens[i]);
			}
			topic_cmd(new_topic, user, user->ptr_channel);
		}
		else
			topic_cmd(tokens, user, user->ptr_channel);
	}
	else if(tokens[0] == "/mode" || tokens[0] == "/mode\n"){
		MODE_command(tokens, user, user->ptr_channel);
	}
	else if(tokens[0] == "inivte" || tokens[0] == "invite\n"){
		invite_cmd(tokens, user, user->ptr_channel);
	}
	else if(user->ptr_channel){//it is a message
		std::string input1 = user->get_name() + ": " + input;
		broadcast_message(user->ptr_channel, user, input1);
	}
	else
		putstr_fd(user->get_socket(), "IRC: Invalid command, use /bot for more information\n");
}
