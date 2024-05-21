#include "../server.hpp"
#include "MODE.hpp"
#include "channel.hpp"
#include <iostream>
#include "Global.hpp"
void channel::add_admin(client_1 *cl)
{
    cl->set_admin(1);
    _admins.push_back(cl);
    std::cout << "Admin added to channel" << std::endl;
}

void channel::remove_admin(client_1 *cl)
{
    cl->set_admin(0);
    std::cout << "Admin removed from channel" << std::endl;
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

int mini_check(std::vector<std::string> option, client_1 *user){
    if(option.size() <= 1){
        send(user->get_socket(), "Not enough options provided\n", strlen("Not enough options provided\n"), 0);
        return 0;
    }
    else if(option.size() > 2){
        send(user->get_socket(), "Too many options provided\n", strlen("Too many options provided\n"), 0);
        return 0;
    }
    return 2;
}

void MODE_command(std::vector<std::string> option, client_1 *user, channel *ch)
{
    if(option[1] != "+k" && option[1] != "-k" && option[1] != "+l" && option[1] != "-l" && option[1] != "+o" && option[1] != "-o"){
        if(mini_check(option, user) == 0){
            std::cout << "mini check failed\n";
            return;
    }
    }
    //invete only  public channel
    if(option[1] == "-i\n"){//remove invete only
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not an admin\n", strlen("You are not an admin\n"), 0);
            return;
        }
        else{
            ch->set_channel_mode(1);//set to public channel
        }
    }
    else if(option[1] == "+i\n"){//set invet only
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not an admin\n", strlen("You are not an admin\n"), 0);
            return;
        }
        else{
            ch->set_channel_mode(0);//set to private channel
        }
    }
    //topic changeable
    else if(option[1] == "+t\n")//anyone can change the topic 1 for yes
    {
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_topic_changeable(1);
        }
    }
    else if(option[1] == "-t\n")//only admin can change the topic 0 for no 
    {
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_topic_changeable(0);
        }
    }
    //password required

    else if(option[1] == "-k\n"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_si_password_required(0);// you can join without password
        }
    }
    
    else if(option[1] == "+k"){
        send(user->get_socket(), "seting password\n", strlen("seting password\n"), 0);
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_si_password_required(1);// you can join with password
            ch->set_password(option[2]);
            send(user->get_socket(), "Password set\n", strlen("Password set\n"), 0);
            send(user->get_socket(), ch->get_password().c_str(), ch->get_password().size(), 0);;
        }
    }

    else if(option[1] == "+o"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            remove_char(option[2], '\n');
            ch->add_admine_by_name(option[2]);
        }
    }

    else if(option[1] == "-o"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            remove_char(option[2], '\n');
            ch->remove_admin_by_name(option[2]);
        }
    }
    else if(option[1] == "-l"){//max client not required
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_is_max_clients_required(1);
        }
    }
    else if(option[1] == "+l"){
        if(ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            std::cout << "setting max clients\n";
            ch->set_is_max_clients_required(0);
            ch->set_number_of_clients(atoi(option[2].c_str()));
        }
    }
    else{
        send(user->get_socket(), "Incorrect option\n", strlen("Incorrect option\n"), 0);
    }
}


//put this in other file
int channel::get_topic_changeable()
{
    return topic_changeable;
}

void topic_cmd(std::vector<std::string> tokens, client_1 *user,  channel *ch)
{
    if(tokens.size() == 1){//veiw topic
        send(user->get_socket(), ch->get_topic().c_str(), ch->get_topic().size(), 0);
    }
    else if(tokens.size() >= 2){//set topic
        if(ch->get_topic_changeable() == 0 && ch->check_if_admin(user) == 0){
            send(user->get_socket(), "You are not allowed\n", strlen("You are not allowed\n"), 0);
            return;
        }
        else{
            ch->set_topic(tokens);
        }
    }
    //in case there is more then one argument change the topic
}

void invite_cmd(std::vector<std::string> tokens, client_1 *user, channel *ch)
{
    if(tokens.size() < 2){
        send(user->get_socket(), "Not enough options provided\n", strlen("Not enough options provided\n"), 0);
        return;
    }
    std::string name = tokens[1];
    for(size_t i = 0; i < all_clients.size(); i++){
        if(all_clients[i]->get_name() == name){
            if(all_clients[i]->ptr_channel != NULL){
                send(user->get_socket(), "User is already in a channel\n", strlen("User is already in a channel\n"), 0);
                return;
            }
            ch->add_client(all_clients[i]);
            send(all_clients[i]->get_socket(), "You have been invited to the channel\n", strlen("You have been invited to the channel\n"), 0);
            //you can leave the channel
            send(all_clients[i]->get_socket(), "You can leave the channel by typing /leave\n", strlen("You can leave the channel by typing /leave\n"), 0);
            return;
        }
    }
    send(user->get_socket(), "User not found\n", strlen("User not found\n"), 0);
}