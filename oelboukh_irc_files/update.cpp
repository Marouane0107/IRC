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
		if(tokens[0] == "/join\n" || tokens[0] == "/join\n" || tokens[0] == "join\n" || tokens[0] == "/JOIN\n" || tokens[0] == "JOIN\n"){
		    std::string msg = "Please enter the channel name after the command >/join #channel_name\n";
		    send(user->get_socket(), msg.c_str(), msg.size(), 0);
		    return;
		}
		join_command(tokens, user);
	}
	
	else if(tokens[0] == "/list\n" || tokens[0] == "list\n" || tokens[0] == "LIST\n" || tokens[0] == "/LIST\n")//if tokens[0] == "/list" list all the users in the channel manually
	{
		if(user->_channels.size() == 0)
			list_command(user->get_socket());
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
				std::string message = "List of channels that you are in \n";
				for(i = 0; i < user->_channels.size(); i++)
				{
					message += "---------------------------------\n";
					message += user->_channels[i]->get_name() + " ";
					message += ft_itos(user->_channels[i]->_clients.size()) + " users\n";
					message += "Topic: " + user->_channels[i]->get_topic() + "\n";
					message += "Channel mode: " + ft_itos(user->_channels[i]->get_channel_mode()) + "\n";
					message += "Password required: " + ft_itos(user->_channels[i]->get_si_password_required()) + "\n";
					message += "---------------------------------\n";
				}
				send(user->get_socket(), message.c_str(), message.size(), 0);
			}
		}
	}
	 else if(tokens[0] == "/kick\n" || tokens[0] == "/kick" || tokens[0] == "kick" || tokens[0] == "THROW" || tokens[0] == "/KICK")
	 {
		if(tokens.size() == 3)
			KICK_command(user, user->_channels, tokens);
		else{

			std::string msg = "This invalid argument >/kick #channel_name user_name  " + ft_itos(tokens.size()) + "\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
			return;
		}
		
	 }

	else if(tokens[0] == "/topic" || tokens[0] == "/topic\n" || tokens[0] == "topic" || tokens[0] == "TOPIC"){
		if(user->_channels.size() == 0)
		{
			std::string msg = "You are not in a channel yet, try to creat one >/join #channel_name\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
		}
		else{
			topic_cmd(tokens, user);
		}
	}
	else if(tokens[0] ==  "/invite\n" || tokens[0] == "/invite" || tokens[0] == "invite" || tokens[0] == "INVITE" || tokens[0] == "/INVITE"){
		if(user->_channels.size() == 0)
		{
			std::string msg = "You are not in a channel yet, try to creat one >/join #channel_name\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
		}
		if(tokens.size() < 3 || tokens.size() > 3)
		{
			std::string msg = "Invalid number of arguments, try >/invite #channel_name user_name\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
		}
		else{
			invite_cmd(tokens, user);
		}
	}
		else if(tokens[0] == "/mode" || tokens[0] == "/mode\n" || tokens[0] == "/MODE" || tokens[0] == "MODE"){
		if(user->_channels.size() == 0){
			std::string msg = "You are not in a channel yet, try to creat one >/join #channel_name\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
		}
		else{
			if((tokens[2] == "-i" || tokens[2] == "+i" || tokens[2] == "+t" || tokens[2] == "-t" ) && tokens.size() > 3){
					send(user->get_socket(), "Error invalid argument:\n", strlen("Error invalid argument:\n"), 0);
			}
			else{
				MODE_command(tokens, user);
			}
		}
	}
	else if(tokens[0] == "/leave" || tokens[0] == "/leave\n" || tokens[0] == "leave" || tokens[0] == "LEAVE"){
		leave_channel(tokens, user);
	}
	else if(tokens[0] == "/chmsg" || tokens[0] == "chmsg" || tokens[0] == "CHMSG" || tokens[0] == "/CHMSG")//try to send message to a channel
	{
		sending_msg(tokens, user);
	}
	else
		putstr_fd(user->get_socket(), "IRC: Invalid command, use /bot for more information\n");

}

 // if(option[1] == "-i\n"){//remove invete only
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not an admin\n", strlen("You are not an admin\n"), 0);
    //         return;
    //     }
    //     else{
    //         ch->set_channel_mode(1);//set to public channel
    //     }
    // }
    // else if(option[1] == "+i\n"){//set invet only
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not an admin\n", strlen("You are not an admin\n"), 0);
    //         return;
    //     }
    //     else{
    //         ch->set_channel_mode(0);//set to private channel
    //     }
    // }
    // //topic changeable
    // else if(option[1] == "+t\n")//anyone can change the topic 1 for yes
    // {
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         ch->set_topic_changeable(1);
    //     }
    // }
    // else if(option[1] == "-t\n")//only admin can change the topic 0 for no 
    // {
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         ch->set_topic_changeable(0);
    //     }
    // }
    // //password required

    // else if(option[1] == "-k\n"){
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         ch->set_si_password_required(0);// you can join without password
    //     }
    // }
    
    // else if(option[1] == "+k"){
    //     send(user->get_socket(), "seting password\n", strlen("seting password\n"), 0);
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         ch->set_si_password_required(1);// you can join with password
    //         ch->set_password(option[2]);
    //         send(user->get_socket(), "Password set\n", strlen("Password set\n"), 0);
    //         send(user->get_socket(), ch->get_password().c_str(), ch->get_password().size(), 0);;
    //     }
    // }

    // else if(option[1] == "+o"){
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         remove_char(option[2], '\n');
    //         ch->add_admine_by_name(option[2]);
    //     }
    // }

    // else if(option[1] == "-o"){
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         remove_char(option[2], '\n');
    //         ch->remove_admin_by_name(option[2]);
    //     }
    // }
    // else if(option[1] == "-l"){//max client not required
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         ch->set_is_max_clients_required(1);
    //     }
    // }
    // else if(option[1] == "+l"){
    //     if(ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         std::cout << "setting max clients\n";
    //         ch->set_is_max_clients_required(0);
    //         ch->set_number_of_clients(atoi(option[2].c_str()));
    //     }
    // }
    // else{
    //     send(user->get_socket(), "Incorrect option\n", strlen("Incorrect option\n"), 0);
    // }