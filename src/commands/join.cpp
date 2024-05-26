#include "../../includes/channel.hpp"
#include "../../includes/server.hpp"
#include "../../includes/Global.hpp"
#include "../../includes/join.hpp"
#include "../../includes/Global.hpp"
#include "../../includes/client.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>


void list_command(int fd)
{
    std::string msg = "<______List of channels you can join_______>\n";
    send(fd, msg.c_str(), msg.size(), 0);
    for(size_t i = 0; i < all_channels.size(); i++)
    {
        if(all_channels[i]->get_channel_mode() == 1)
        {
            int number_of_clients = all_channels[i]->_clients.size();
            std::string msg = all_channels[i]->get_name() + " | " + ft_itos(number_of_clients) + " users\n";
            send(fd, msg.c_str(), msg.size(), 0);
        }
    }
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
    std::vector<client_1*>::iterator it;//send the message to all the clients in the channel
    for(it = ch->_clients.begin(); it != ch->_clients.end(); it++){
        if((*it)->get_socket() != sender->get_socket()){
                send((*it)->get_socket(), message.c_str(), message.size(), 0);
        }
    }
}

void chat_section(channel *ch, client_1 *sender, const std::string &message) {
    std::string msg = sender->get_name() + ": " + message;
    broadcast_message(ch, sender, msg);
}

void send_to_all_clients_in_channel(channel *ch, client_1 *user){
    std::vector<client_1*>::iterator it;
    for(it = ch->_clients.begin(); it != ch->_clients.end(); it++){
        if((*it)->get_socket() != user->get_socket()){
            annonce_user_channel((*it)->get_socket(), ch->get_name(), user->get_name());//recheck
        }
    }
}
void join_command(std::vector<std::string> tokens, client_1 *user)
{
    if(tokens[tokens.size() - 1] == "\n"){
        tokens.pop_back();
    }

    if(tokens.size() < 2){
        std::string msg = "wrong number of arguments or invalid\n";
        send(user->get_socket(), msg.c_str(), msg.size(), 0);
        return;
    }
    std::string channel_name = tokens[1];
    if(channel_name == "#\n"){//need to recheck
        send(user->get_socket(), "Invalid channel name\n", strlen("Invalid channel name\n"), 0);
        return;
    }
    else if(channel_name == "#"){
        send(user->get_socket(), "Invalid channel name /join #channel_name\n", strlen("Invalid channel name /join #channel_name\n"), 0);
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
        lst_add_back(ch);
        user->set_admin(1);
        user->set_super_admin(1);
        ch->add_admin(user);
        ch->add_client(user);
        all_channels.push_back(ch);
        user->_channels.push_back(ch);
        std::string msg = "Channel created, you are the admin\n";
        send(user->get_socket(), msg.c_str(), msg.size(), 0);
        return;
    }

    if(std::find(ch->_clients.begin(), ch->_clients.end(), user) != ch->_clients.end()){
            send(user->get_socket(), "You are already in this channel\n", strlen("You are already in this channel\n"), 0);
    }
    else{
        remove_char(tokens[2], '\n');
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
            user->_channels.push_back(ch);
            // std::string message = user->get_name() + " has joined the channel" + ch->get_name() + "\n";
            // send(user->get_socket(), message.c_str(), message.size(), 0);
            // broadcast_message(ch, user, message);
            welcome_user_channel(user->get_socket(), ch->get_name());
            send_to_all_clients_in_channel(ch, user);
        }
        else{
            send(user->get_socket(), "Channel is full, or it is invite only\n", strlen("Channel is full, or it is invite only\n"), 0);
        }
}

}
client_1* return_new_admine(channel *ch, client_1 old_admine){
    std::vector<client_1*>::iterator it;
    for(it = ch->_clients.begin(); it != ch->_clients.end(); it++){
        if((*it)->get_name() != old_admine.get_name()){ 
            return *it;
        }
    }
    return NULL;
}

void remove_channel_from_list_of_the_user_channels(client_1 *user, channel *ch)
{
    user->_channels.erase(std::remove(user->_channels.begin(), user->_channels.end(), ch), user->_channels.end());
}

void remove_user_from_channel(channel *ch, client_1 *user){
    ch->_clients.erase(std::remove(ch->_clients.begin(), ch->_clients.end(), user), ch->_clients.end());
}

void send_to_all_users_in_channel(channel *ch, client_1 *user){
    std::vector<client_1*>::iterator it;
    for(it = ch->_clients.begin(); it != ch->_clients.end(); it++){
        if((*it)->get_socket() != user->get_socket()){
            annonce_user_channel_leave((*it)->get_socket(), ch->get_name(), user->get_nick());
        }
    }
}

void leave_channel(std::vector<std::string> tokens, client_1 *user){
    std::string name = tokens[1];
    remove_char(name, '\n');
    channel *ch = search_for_channel(name);
    if(ch == NULL){
        send(user->get_socket(), "You are not in the channel\n", 28, 0);
        return;
    }
    if(std::find(ch->_clients.begin(), ch->_clients.end(), user) != ch->_clients.end()){
        send_to_all_users_in_channel(ch, user);
        remove_channel_from_list_of_the_user_channels(user, ch);
        remove_user_from_channel(ch, user);
        goodbye_user_channel(user->get_socket(), ch->get_name());
        if(ch->_clients.size() == 0){
            all_channels.erase(std::remove(all_channels.begin(), all_channels.end(), ch), all_channels.end());
        }
        
        if(user->get_super_admin() == 1 && ch->_clients.size() >= 1){
            ch->remove_admin(user);
            client_1 *new_admine = return_new_admine(ch, *user);
            if(new_admine != NULL){
                new_admine->set_super_admin(1);
                new_admine->set_admin(1);
                ch->add_admin(new_admine);
                std::string message = "You are now the admin of the channel\n";
                send(new_admine->get_socket(), message.c_str(), message.size(), 0);
                //brodcast to the channel
                std::string message2 = new_admine->get_name() + "-----------> is the new admin of the channel\n";
                broadcast_message(ch, new_admine, message2);
            }
        }
    }
    else{
        send(user->get_socket(), "You are not in the channel\n", 28, 0);
    }
}

int search_for_user_in_channel_by_name(client_1* user, std::string name){
    std::vector<channel*>::iterator it;

    it = user->_channels.begin();
    for(it = user->_channels.begin(); it != user->_channels.end(); it++){
        if((*it)->get_name() == name){
            return 1;
        }
    }
    return 0;
}
int check_if_channel_has_super_admin(channel *ch){
    std::vector<client_1*>::iterator it;
    for(it = ch->_clients.begin(); it != ch->_clients.end(); it++){
        if((*it)->get_super_admin() == 1){
            return 1;
        }
    }
    return 0;
}
void leave_channels(std::string name,  client_1 *user){
    channel *ch = search_for_channel(name);
    if(ch == NULL){
        return;
    }
    if(std::find(ch->_clients.begin(), ch->_clients.end(), user) != ch->_clients.end()){
        send_to_all_users_in_channel(ch, user);
        remove_channel_from_list_of_the_user_channels(user, ch);
        remove_user_from_channel(ch, user);
        if(ch->_clients.size() == 0){
            all_channels.erase(std::remove(all_channels.begin(), all_channels.end(), ch), all_channels.end());
        }
        if(user->get_super_admin() == 1 && ch->_clients.size() >= 1){
            client_1 *new_admine = return_new_admine(ch, *user);
            if(new_admine != NULL /*&& !search_for_user_in_channel_by_name(new_admine, ch->get_name())*/){
                new_admine->set_super_admin(1);
                new_admine->set_admin(1);
                ch->add_admin(new_admine);
                ch->get_admins().push_back(new_admine);
                std::string message = "You are now the admin of the channel---------------->\n";
                // send(new_admine->get_socket(), message.c_str(), message.size(), 0);
                //brodcast to the channel
                std::string message2 = new_admine->get_name() + "-----------> is the new admin of the channel\n";
                broadcast_message(ch, new_admine, message2);
            }//need to be fixed
        //    if( check_if_channel_has_super_admin(ch) == 0){
        //         //setting the first user in the channel as the super admin
        //         std::vector<client_1*>::iterator it;
        //         for(it = ch->_clients.begin(); it != ch->_clients.end(); it++){
        //             if((*it)->get_super_admin() == 0){
        //                 (*it)->set_super_admin(1);
        //                 (*it)->set_admin(1);
        //                 ch->add_admin(*it);
        //                 ch->get_admins().push_back(*it);
        //                 std::string message = "You are now the super admin of the channel\n";
        //                 send((*it)->get_socket(), message.c_str(), message.size(), 0);
        //                 break;
        //             }
        //         }
        //         return;
        //     }
           }
        }
}
