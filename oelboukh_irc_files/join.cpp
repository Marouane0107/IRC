#include "channel.hpp"
#include "../server.hpp"
#include "join.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "Global.hpp"


void list_command(int fd, channel *ch)
{
    std::string message = "List of users in the channel:\n";
            size_t i;
            for(i = 0; i < ch->_clients.size(); i++){
                std::cout << ch->_clients[i]->get_name() << std::endl;
                message += ch->_clients[i]->get_name() + "\n";
            }
            send(fd, message.c_str(), message.size(), 0);
}

std::vector<std::string> split(const std::string& str, char delimiter){
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)){
        if(!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}


channel *search_for_channel(std::string name)
{
    for (size_t i = 0; i < all_channels.size(); i++)
    {
        if(all_channels[i]->get_name() == name)
        {
            return all_channels[i];
        }
    }
    return NULL;
}
//
void broadcast_message(channel *ch, client_1 *sender, const std::string &message) {
    std::vector<client_1*>::iterator it;
    for (it = ch->_clients.begin(); it != ch->_clients.end(); it++){
        if ((*it) != sender) {
            send((*it)->get_socket(), message.c_str(), message.size(), 0);//send the message to all the users in the channel
        }
    }
}

void chat_section(channel *ch, client_1 *sender, const std::string &message) {
    std::string msg = sender->get_name() + ": " + message;
    broadcast_message(ch, sender, msg);
}
void tmp_print_vecttor(std::vector<std::string> tokens){
    for(size_t i = 0; i < tokens.size(); i++){
        std::cout << tokens[i] << std::endl;
    }
}

void remove_user_from_all_channels(client_1 *user){
    for(size_t i = 0; i < all_channels.size(); i++){
        std::vector<client_1*>::iterator it;
        it = std::find(all_channels[i]->_clients.begin(), all_channels[i]->_clients.end(), user);
        if(it != all_channels[i]->_clients.end()){
            all_channels[i]->_clients.erase(it);
        }
    }  
}
void join_command(std::vector<std::string> tokens, client_1 *user)
{
    
    if(tokens.size() < 2 || tokens.size() > 3){
        std::string msg = "wrong number of arguments or invalid\n";
        send(user->get_socket(), msg.c_str(), msg.size(), 0);
        return;
    }
    std::string channel_name = tokens[1];
    if(channel_name == "#\n"){
        send(user->get_socket(), "Invalid channel name\n", strlen("Invalid channel name\n"), 0);
        return;
    }
    remove_char(channel_name, '\n');
    channel *ch = search_for_channel(channel_name);
    if(ch == NULL)
    {   if(channel_name[0] != '#'){
            send(user->get_socket(), "Invalid channel name\n", strlen("Invalid channel name\n"), 0);
            return;
        }
        else if(tokens[1].empty()){
            send(user->get_socket(), "Channel name required\n", strlen("Channel name required\n"), 0);
            return;
        }
        ch = new channel(channel_name);
        user->set_admin(1);
        user->set_super_admin(1);
        remove_user_from_all_channels(user);
        ch->add_admin(user);
        ch->add_client(user);
        all_channels.push_back(ch);
        user->ptr_channel = ch;
        std::string msg = "Channel created, you are the admin\n";
        send(user->get_socket(), msg.c_str(), msg.size(), 0);
        return;
    }

    if(std::find(ch->_clients.begin(), ch->_clients.end(), user) != ch->_clients.end()){
            send(user->get_socket(), "You are already in this channel\n", strlen("You are already in this channel\n"), 0);
    }
    else{
        if(ch->get_si_password_required() == 1){
            if(tokens.size() < 3){
                send(user->get_socket(), "Password required\n", strlen("Password required\n"), 0);
                return;
            }
            else if(ch->get_password() != tokens[2]){
                send(user->get_socket(), "Incorrect password\n", strlen("Incorrect password\n"), 0);
                return;
            }
        }
        if(ch->get_channel_mode() == 1 && ch->_clients.size() < ch->get_number_of_clients()){
        ch->add_client(user);
        user->ptr_channel = ch;
        std::string message = user->get_name() + " has joined the channel\n";
        broadcast_message(ch, user, message);
        send(user->get_socket(), "You have joined the channel\n", strlen("You have joined the channel\n"), 0);
        }
        else{
            send(user->get_socket(), "Channel is full, or it is invite only\n", strlen("Channel is full, or it is invite only\n"), 0);
        }
}

//whne use ctrl + c to exit the program the user will be removed from the channel 
    // while(1)
    // {
    //     std::vector<std::string> tokens;
    //     memset(cl->cmd, 0, sizeof(cl->cmd));
    //     send(cl->socket, ">", strlen(">"), 0);
    //     keep_recv(cl, cl->cmd);
    //     tokens = split(cl->cmd, ' ');
    //     if(tokens[0] == "/quit\n")
    //     {
    //         quit_command(ch, user);
    //         break;
    //     }
    //     else if(tokens[0] == "/KICK"){
    //         //check if the user is an admin
    //         KICK_command(cl, user, ch);
    //     }
    //     else if(tokens[0] == "/list\n")
    //     {
    //         list_command(cl,ch);
    //     }
    //     else if(tokens[0] == "/MODE" || tokens[0] == "/MODE\n")
    //     {
    //         MODE_command(tokens, user, ch);
    //     }
    //     // std::cout << "[43]-----------------------------topic command\n";
    //     else if(tokens[0] == "/TOPIC\n" || tokens[0] == "/TOPIC"){
    //         topic_cmd(tokens, user, ch);
    //     }
    //     else if(tokens[0] == "/INVITE"){
    //         invite_cmd(tokens, user, ch);
    //     }
    //     else
    //     {
    //         std::string message = user->get_name() + ": " + cl->cmd;
    //         broadcast_message(ch, user, message);
    //     }
    //     std::string message = cl->cmd;
    //     chat_section(ch, user, message);
    // }
}