#include "../../includes/client.hpp"
#include "../../includes/server.hpp"
#include "../../includes/channel.hpp"
#include "../../includes/Global.hpp"
#include "../../includes/MODE.hpp"
#include "../../includes/join.hpp"


int client_1::get_super_admin(){
	return this->super_admin;
}
void client_1::set_super_admin(int super_admin){
	this->super_admin = super_admin;
}
void update_nick_command(std::string new_nick, std::string old_nick)
{
	size_t i;
	for(i = 0; i < all_clients.size(); i++)//change nick name
	{
		if(all_clients[i] != NULL){
			if(all_clients[i]->get_nick() == old_nick)
			{
				all_clients[i]->set_nick(new_nick);
				return;
			}
		}
	}
}

void update_user_command(std::string new_user, std::string old_user)
{
	size_t i;
	for(i = 0; i < all_clients.size(); i++)//change user name
	{
		if(all_clients[i] != NULL){
			if(all_clients[i]->get_name() == old_user)
			{
				all_clients[i]->set_name(new_user);
				return;
			}
		}
	}
}

void update_real_name_command(std::string real_name, std::string nick)
{
	size_t i;
	for(i = 0; i < all_clients.size(); i++)//change real name
	{
		if(all_clients[i] != NULL){
			if(all_clients[i]->get_real_name() == nick)
			{
				all_clients[i]->set_real_name(real_name);
				return;
			}
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
					if(user->_channels[i]->get_channel_mode() == 0)
						message += "Channel mode: Invite only\n";
					else
						message += "Channel mode: Public\n";
					if(user->_channels[i]->get_si_password_required() == 0)
						message += "Password required: No\n";
					else
						message += "Password required: Yes\n";
					message += "---------------------------------\n";
				}
				send(user->get_socket(), message.c_str(), message.size(), 0);
			}
		}
	}
	 else if(tokens[0] == "/kick\n" || tokens[0] == "/kick" || tokens[0] == "kick" || tokens[0] == "THROW" || tokens[0] == "/KICK" || tokens[0] == "KICK" || tokens[0] == "throw" || tokens[0] == "/THROW")
	 {
		if(tokens[tokens.size() - 1] == "\n")
		{
			tokens.pop_back();
		}
		if(tokens.size() == 3)
			KICK_command(user, user->_channels, tokens);
		else{

			std::string msg = "This invalid argument >/kick #channel_name user_name  " + ft_itos(tokens.size()) + "\n";
			send(user->get_socket(), msg.c_str(), msg.size(), 0);
			return;
		}
		
	 }

	else if(tokens[0] == "/topic" || tokens[0] == "/topic\n" || tokens[0] == "topic" || tokens[0] == "TOPIC"){
		if(tokens[tokens.size() - 1] == "\n")
		{
			tokens.pop_back();
		}
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
			if((tokens[2] == "-i" || tokens[2] == "+i" || tokens[2] == "+t" || tokens[2] == "-t" || tokens[2] == "-k") && tokens.size() > 3 && tokens[3] != "\n"){
					send(user->get_socket(), "Error invalid argument:\n", strlen("Error invalid argument:\n"), 0);
			}
			else{
				MODE_command(tokens, user);
			}
		}
	}
	else if(tokens[0] == "/leave" || tokens[0] == "/leave\n" || tokens[0] == "leave" || tokens[0] == "LEAVE" || tokens[0] == "PART" || tokens[0] == "/PART")
		leave_channel(tokens, user);
	else if(tokens[0] == "/chmsg" || tokens[0] == "chmsg" || tokens[0] == "CHMSG" || tokens[0] == "/CHMSG")//try to send message to a channel
		sending_msg(tokens, user);
	else
		putstr_fd(user->get_socket(), "IRC: Invalid command, use /bot for more information\n");

}
