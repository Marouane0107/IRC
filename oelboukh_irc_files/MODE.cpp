#include "../server.hpp"
#include "MODE.hpp"
#include "channel.hpp"
#include "join.hpp"
#include <iostream>
#include "Global.hpp"
void channel::add_admin(client_1 *cl)
{
    cl->set_admin(1);
    _admins.push_back(cl);
    putstr_fd(cl->get_socket(), "You are now an admin\n");
}

void channel::remove_admin(client_1 *cl)
{   if(cl->get_admin() == 1){
    cl->set_admin(0);
    if(cl->get_super_admin() == 1){
        cl->set_super_admin(0);
    }
    putstr_fd(cl->get_socket(), "You are no longer an admin\n");
    for(size_t i = 0; i < _admins.size(); i++){
        if(_admins[i] == cl){
            _admins.erase(_admins.begin() + i);
        }
    }
    }
    else
        return;
}

void channel::set_topic(std::vector<std::string> topic)
{
    std::string msg;
    for(size_t i = 1; i < topic.size(); i++){
        msg += topic[i] + " ";
    }
    _topic = msg;
}

void channel::set_password(std::string password)
{
    _password = password;
    std::cout << "Password set to: " << _password << std::endl;
}

void channel::set_channel_mode(int mode)
{
    channel_mode = mode;
    if(mode == 1){
        std::cout << "Channel mode set to: Public" << std::endl;
    }
    else if(mode == 0){
        std::cout << "Channel mode set to: Private" << std::endl;
    }
    else{//print helping message
        std::cout << "incorrect mode " << std::endl;
    }
    std::cout << "Channel mode set to: " << channel_mode << std::endl;
}

void channel::set_si_password_required(int si_password_required)
{
    this->si_password_required = si_password_required;
    if(si_password_required == 0){
        std::cout << "Password not required" << std::endl;
    }
    else if(si_password_required == 1){
        std::cout << "Password required" << std::endl;
    }
    else{//print helping message
        std::cout << "incorrect mode " << std::endl;
    }
}

void channel::set_name(std::string name)
{
    _name = name;
    std::cout << "Channel name set to: " << _name << std::endl;
}

std::string channel::get_topic()
{
    return _topic;
}

std::string channel::get_password()
{
    return _password;
}

int channel::get_channel_mode()
{
    return channel_mode;
}

int channel::get_si_password_required()
{
    return si_password_required;
}

// int mini_check(std::vector<std::string> option, client_1 *user){
//     if(option.size() <= 1){
//         putstr_fd(user->get_socket(), "Not enough options provided\n", strlen("Not enough options provided\n"), 0);
//         return 0;
//     }
//     else if(option.size() > 2){
//         putstr_fd(user->get_socket(), "Too many options provided\n", strlen("Too many options provided\n"), 0);
//         return 0;
//     }
//     return 2;
// }
// /MODE #channel_mode +i 
// /MODE #channel_mode -i
int search_for_user_in_channel(channel *ch, client_1 *cl)
{
    for(size_t i = 0; i < ch->_clients.size(); i++){
        if(ch->_clients[i] == cl){
            return 1;
        }
    }
    return 0;
}
int check_all_if_digit(std::string str)
{
    remove_char(str, '\n');
    for(size_t i = 0; i < str.size(); i++){
        if(std::isdigit(str[i]) == 0 ){
            return 0;
        }
    }
    return 1;
}
void MODE_command(std::vector<std::string> option, client_1 *user)
{
    std::string name = option[1];
    remove_char(name, '\n');
    channel *ch = search_for_channel(name);
    if(ch == NULL){
        putstr_fd(user->get_socket(), "Channel not found\n");
        return;
    }
    if(check_if_user_in_channel(user, ch) == 0){
        putstr_fd(user->get_socket(), "You are not in the channel\n");
        return;
    }
    if(option[2] == "+i\n" || option[2] == "+i"){//invite only mode to channel
        if(ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
        else{
            ch->set_channel_mode(0);
            putstr_fd(user->get_socket(), "Channel mode set to: Invite only\n");
        }
    }
    else if(option[2] == "-i\n" || option[2] == "-i"){//public mode to channel
        if(ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
        else{
            ch->set_channel_mode(1);
            putstr_fd(user->get_socket(), "Channel mode set to: Public\n");
        }
    }
    else if(option[2] == "-t\n"|| option[2] == "-t"){
        if(ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
        else{
            ch->set_topic_changeable(0);
            putstr_fd(user->get_socket(), "Topic is not changeable\n");
        }
    }
    else if(option[2] == "+t\n" || option[2] == "+t"){
        if(ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
    else{
        ch->set_topic_changeable(1);
        putstr_fd(user->get_socket(), "Topic is changeable\n");
    }
    }
    else if(option[2] == "+k"){
        if(ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
        else{
            ch->set_si_password_required(1);// you can join with password
            remove_char(option[3], '\n');
            ch->set_password(option[3]);
            putstr_fd(user->get_socket(), "Password set\n");
        }
    }
    else if(option[2] == "-k\n" || option[2] == "-k"){
        if(ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
        else{
            ch->set_si_password_required(0);// you can join without password
            putstr_fd(user->get_socket(), "Password removed\n");
        }
    }
    else if(option[2] == "+o"){
        std::string name = option[3];
        remove_char(name, '\n');
        client_1 *cl = search_for_client(name);
        if(cl == NULL){
            putstr_fd(user->get_socket(), "User not found\n");
            return;
        }//check if user is in the channel
        if(search_for_user_in_channel(ch, cl) == 0){
            putstr_fd(user->get_socket(), "User is not in the channel\n");
            return;
        }
        if(ch->check_if_admin(cl) == 1){
            putstr_fd(user->get_socket(), "User is already an admin\n");
            return;
        }
        ch->add_admin(cl);
        cl->set_admin(1);
        putstr_fd(user->get_socket(), "User is now an admin\n");
    }
    else if(option[2] == "-o"){
        std::string name = option[3];
        remove_char(name, '\n');
        client_1 *cl = search_for_client(name);
        if(cl == NULL){
            putstr_fd(user->get_socket(), "User not found\n");
            return;
        }//check if user is in the channel
        if(search_for_user_in_channel(ch, cl) == 0){
            putstr_fd(user->get_socket(), "User is not in the channel\n");
            return;
        }
        if(ch->check_if_admin(cl) == 0){
            putstr_fd(user->get_socket(), "User is not an admin\n");
            return;
        }
        ch->remove_admin(cl);
        cl->set_admin(0);
        putstr_fd(user->get_socket(), "User is no longer an admin\n");
    }
    else if(option[2] == "-l" || option[2] == "-l\n"){//max client not required
        if(ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
        else{
            ch->set_is_max_clients_required(1);
            ch->set_number_of_clients(MAX_CLIENTS);
            putstr_fd(user->get_socket(), "Max clients not required\n");
        }
    }
    else if(option[2] == "+l"){//max client not required
        //check if argument is digits
        if(check_all_if_digit(option[3]) == 0){
            putstr_fd(user->get_socket(), "Incorrect argument\n");
            return;
        }
        int max_clients = atoi(option[3].c_str());
        if(ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
        else{
            ch->set_is_max_clients_required(0);
            ch->set_number_of_clients(max_clients);
            putstr_fd(user->get_socket(), "Max clients set\n");
        }
    }
    else{
        putstr_fd(user->get_socket(), "Incorrect mode\n");
    }

}


//put this in other file
int channel::get_topic_changeable()
{
    return topic_changeable;
}

void topic_cmd(std::vector<std::string> tokens, client_1 *user)//need to chenge 
{
    std::string name = tokens[1];
    remove_char(name, '\n');
    channel *ch = search_for_channel(name);
    if(ch == NULL){
        putstr_fd(user->get_socket(), "Channel not found\n");
        return;
    }
    if(check_if_user_in_channel(user, ch) == 0){
        putstr_fd(user->get_socket(), "You are not in the channel\n");
        return;
    }
    if(tokens.size() == 2){//get topic
        std::string msg = "The topic is: " + ch->get_topic() + "\n";
        putstr_fd(user->get_socket(), msg.c_str());
    }
    if(tokens.size() > 2){//set topic
        if(ch->get_topic_changeable() == 0 && ch->check_if_admin(user) == 0){
            putstr_fd(user->get_socket(), "You are not allowed\n");
            return;
        }
        else{
            std::cout << user->get_admin() << std::endl;
            std::cout << ch->get_topic_changeable() << std::endl;
            std::cout << ch->check_if_admin(user) << std::endl;
            std::vector<std::string> topic;
            for(size_t i = 1; i < tokens.size(); i++){
                topic.push_back(tokens[i]);
            }
            ch->set_topic(topic);
        }
    }
}
client_1 *search_for_client(std::string name)
{
    for(size_t i = 0; i < all_clients.size(); i++){
        if(all_clients[i]->get_name() == name){
            return all_clients[i];
        }
    }
    return NULL;
}
void invite_cmd(std::vector<std::string> tokens, client_1 *user)//check if you are adding an removing chaneels after adding the user or kicking or removing the user
{
    std::string name = tokens[1];
    std::string user_name = tokens[2];
    client_1 *user_invited = NULL;
    remove_char(name, '\n');
    remove_char(user_name, '\n');
    channel *ch = search_for_channel(name);
    if(check_if_user_in_channel(user, ch) == 0){
        if(ch == NULL){
            putstr_fd(user->get_socket(), "Channel not found\n");
            return;
        }
        putstr_fd(user->get_socket(), "You are not in the channel\n");
        return;
    }
    else{
        user_invited = search_for_client(user_name);
        if(user_invited == NULL){
            putstr_fd(user->get_socket(), "User not found\n");
            return;
        }
        if(check_if_user_in_channel(user_invited, ch) == 1){
            putstr_fd(user->get_socket(), "User is already in the channel\n");
            return;
        }
        ch->add_client(user_invited);
        user_invited->_channels.push_back(ch);
        send_to_all_clients_in_channel(ch, user_invited);
        putstr_fd(user_invited->get_socket(), "IRC: You have been invited to the channel\n");
        putstr_fd(user_invited->get_socket(), "IRC: You can leave the channel by typing </leave #channel_name>\n");
    }
}

void sending_msg(std::vector<std::string> tokens, client_1 *user)
{
    std::string   name  = tokens[1];
    remove_char(name, '\n');
    channel *ch = search_for_channel(name);
    if(ch == NULL){
        putstr_fd(user->get_socket(), "IRC: Channel not found\n");
        return;
    }
    if(check_if_user_in_channel(user, ch) == 0){
        putstr_fd(user->get_socket(), "IRC: You are not in the channel\n");
        return;
    }
    std::string msg;
    msg += "------------------NOTIFICATIONS-------------------\n";
    msg += "|> ";
    msg += "from channel: " + name + " ";
    msg += user->get_name() + ": ";
    for(size_t i = 2; i < tokens.size(); i++){
        msg += tokens[i] + " ";
    }
    msg += "--------------------------------------------------\n";
    for(size_t i = 0; i < ch->_clients.size(); i++){
        if(ch->_clients[i]->get_socket() != user->get_socket())
            putstr_fd(ch->_clients[i]->get_socket(), msg.c_str());
    }
}