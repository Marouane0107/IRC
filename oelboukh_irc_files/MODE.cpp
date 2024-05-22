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
    send(cl->get_socket(), "You are now an admin\n", strlen("You are now an admin\n"), 0);
}

void channel::remove_admin(client_1 *cl)
{
    cl->set_admin(0);
    send(cl->get_socket(), "You are no longer an admin\n", strlen("You are no longer an admin\n"), 0);
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
//         send(user->get_socket(), "Not enough options provided\n", strlen("Not enough options provided\n"), 0);
//         return 0;
//     }
//     else if(option.size() > 2){
//         send(user->get_socket(), "Too many options provided\n", strlen("Too many options provided\n"), 0);
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
void MODE_command(std::vector<std::string> option, client_1 *user)
{
    // if(option[1] != "+k" && option[1] != "-k" && option[1] != "+l" && option[1] != "-l" && option[1] != "+o" && option[1] != "-o"){
    //     if(mini_check(option, user) == 0){
    //         std::cout << "mini check failed\n";
    //         return;
    // }
    // }
   
    //handel /mode #channel +i
    std::string name = option[1];
    remove_char(name, '\n');
    channel *ch = search_for_channel(name);
    if(ch == NULL){
        send(user->get_socket(), "Channel not found\n", strlen("Channel not found\n"), 0);
        return;
    }
    if(check_if_user_in_channel(user, ch) == 0){
        send(user->get_socket(), "You are not in the channel\n", strlen("You are not in the channel\n"), 0);
        return;
    }
    if(option[2] == "+i\n"){//invite only mode to channel
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_channel_mode(0);
        }
    }
    else if(option[2] == "-i\n"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_channel_mode(1);
        }
    }
    else if(option[2] == "-t\n"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_topic_changeable(0);
        }
    }
    else if(option[2] == "+t\n"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
    else{
        ch->set_topic_changeable(1);
    }
    }
    else if(option[2] == "+k"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_si_password_required(1);// you can join with password
            ch->set_password(option[3]);
            send(user->get_socket(), "Password set\n", strlen("Password set\n"), 0);
            send(user->get_socket(), ch->get_password().c_str(), ch->get_password().size(), 0);
        }
    }
    else if(option[2] == "-k\n"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_si_password_required(0);// you can join without password
        }
    }
    else if(option[2] == "+o"){
        std::string name = option[3];
        remove_char(name, '\n');
        client_1 *cl = search_for_client(name);
        if(cl == NULL){
            send(user->get_socket(), "User not found\n", strlen("User not found\n"), 0);
            return;
        }//check if user is in the channel
        if(search_for_user_in_channel(ch, cl) == 0){
            send(user->get_socket(), "User is not in the channel\n", strlen("User is not in the channel\n"), 0);
            return;
        }
        if(ch->check_if_admin(cl) == 1){
            send(user->get_socket(), "User is already an admin\n", strlen("User is already an admin\n"), 0);
            return;
        }
        ch->add_admin(cl);
        cl->set_admin(1);
    }
    else if(option[2] == "-o"){
        std::string name = option[3];
        remove_char(name, '\n');
        client_1 *cl = search_for_client(name);
        if(cl == NULL){
            send(user->get_socket(), "User not found\n", strlen("User not found\n"), 0);
            return;
        }//check if user is in the channel
        if(search_for_user_in_channel(ch, cl) == 0){
            send(user->get_socket(), "User is not in the channel\n", strlen("User is not in the channel\n"), 0);
            return;
        }
        if(ch->check_if_admin(cl) == 0){
            send(user->get_socket(), "User is not an admin\n", strlen("User is not an admin\n"), 0);
            return;
        }
        ch->remove_admin(cl);
        cl->set_admin(0);
    }
    else if(option[2] == "-l"){//max client not required
        int max_clients = atoi(option[3].c_str());
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_is_max_clients_required(1);
            ch->set_number_of_clients(max_clients);
        }
    }
    else if(option[2] == "+l"){//max client not required
        int max_clients = atoi(option[3].c_str());
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_is_max_clients_required(0);
            ch->set_number_of_clients(max_clients);
        }
    }
    else{
        send(user->get_socket(), "Incorrect mode\n", strlen("Incorrect mode\n"), 0);
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
        send(user->get_socket(), "Channel not found\n", strlen("Channel not found\n"), 0);
        return;
    }
    if(check_if_user_in_channel(user, ch) == 0){
        send(user->get_socket(), "You are not in the channel\n", strlen("You are not in the channel\n"), 0);
        return;
    }
    if(tokens.size() == 2){//get topic
        std::string msg = "The topic is: " + ch->get_topic() + "\n";
        send(user->get_socket(), msg.c_str(), msg.size(), 0);
    }
    if(tokens.size() > 2){//set topic
        if(ch->get_topic_changeable() == 0 && ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
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
    // else if(tokens.size() >= 2){//set topic
    //     if(ch->get_topic_changeable() == 0 && ch->check_if_admin(user) == 0){
    //         send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
    //         return;
    //     }
    //     else{
    //         ch->set_topic(tokens);
    //     }
    // }
    //in case there is more then one argument change the topic
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
            send(user->get_socket(), "Channel not found\n", strlen("Channel not found\n"), 0);
            return;
        }
        send(user->get_socket(), "You are not in the channel\n", strlen("You are not in the channel\n"), 0);
        return;
    }
    else{
        user_invited = search_for_client(user_name);
        if(user_invited == NULL){
            send(user->get_socket(), "User not found\n", strlen("User not found\n"), 0);
            return;
        }
        if(check_if_user_in_channel(user_invited, ch) == 1){
            send(user->get_socket(), "User is already in the channel\n", strlen("User is already in the channel\n"), 0);
            return;
        }
        ch->add_client(user_invited);
        user_invited->_channels.push_back(ch);
        send(user_invited->get_socket(), "You have been invited to the channel\n", strlen("You have been invited to the channel\n"), 0);
        send(user_invited->get_socket(), "You can leave the channel by typing /leave\n", strlen("You can leave the channel by typing /leave\n"), 0);
    }
}

void sending_msg(std::vector<std::string> tokens, client_1 *user)
{
    std::string   name  = tokens[1];
    remove_char(name, '\n');
    channel *ch = search_for_channel(name);
    if(ch == NULL){
        send(user->get_socket(), "Channel not found\n", strlen("Channel not found\n"), 0);
        return;
    }
    if(check_if_user_in_channel(user, ch) == 0){
        send(user->get_socket(), "You are not in the channel\n", strlen("You are not in the channel\n"), 0);
        return;
    }
    std::string msg;
    msg = "from channel: " + name + " ";
    msg += user->get_name() + ": ";
    for(size_t i = 2; i < tokens.size(); i++){
        msg += tokens[i] + " ";
    }
    msg += "\n";
    for(size_t i = 0; i < ch->_clients.size(); i++){
        if(ch->_clients[i]->get_socket() != user->get_socket())
            send(ch->_clients[i]->get_socket(), msg.c_str(), msg.size(), 0);
    }
}