#include "join.hpp"
#include <sstream>
#include "channel.hpp"
#include "../server.hpp"
#include "Global.hpp"
//remove admis accses to the channel
void remove(channel *ch, client_1 *user)
{
    std::vector<client_1*> arr = ch->get_admins();
    for(size_t i = 0; i < arr.size(); i++){
        if(arr[i]->get_socket() == user->get_socket()){
            ch->remove_admin(user);
            std::string message = "You are no longer an admin\n";
            send(user->get_socket(), message.c_str(), message.size(), 0);
            break;
        }
    }
} 
void quit_command(channel *ch, client_1 *user)
{
           //if the user is an admin, give the admin to another user
            if(user->get_admin() == 1){
                std::vector<client_1*>::iterator it;
                
                for (it = ch->_clients.begin(); it != ch->_clients.end(); it++){
                    if((*it)->get_socket() != user->get_socket()){
                        (*it)->set_admin(1);
                        ch->add_admin(*it);
                        std::string message = "You are now the admin\n";
                        send((*it)->get_socket(), message.c_str(), message.size(), 0);
                        break;
                    }
                    //if there is only one user in the channel
                    else{
                        ch->_clients.erase(std::remove(ch->_clients.begin(), ch->_clients.end(), user), ch->_clients.end());
                        all_channels.erase(std::remove(all_channels.begin(), all_channels.end(), ch), all_channels.end());
                        std::string msg = "channel has been deleted and \n";
                        send(user->get_socket(), msg.c_str(), msg.size(), 0);
                        break;
                    }
                }
            }
            // user->ptr_channel = NULL;// rmove channel form channel list in the uaser list of channels 
            user->set_admin(0);
            remove(ch, user);
            std::string message = user->get_name() + " has left the channel\n";
            broadcast_message(ch, user, message);
            ch->_clients.erase(std::remove(ch->_clients.begin(), ch->_clients.end(), user), ch->_clients.end());
}
void remove_char(std::string &str, char ch)
{
    size_t i = 0;
    while(i < str.size())
    {
        if(str[i] == ch)
        {
            str.erase(i, 1);
        }
        else
        {
            i++;
        }
    }
}
int chacke_if_addmin(client_1 *user, channel *ch)
{
    std::vector<client_1*> arr = ch->get_admins();
    for(size_t i = 0; i < arr.size(); i++){
        if(arr[i]->get_socket() == user->get_socket()){
            return 1;
        }
    }
    return 0;
}
int check_if_user_in_channel(client_1 *user, channel *ch)
{
    std::vector<client_1*>::iterator it;
    for (it = ch->_clients.begin(); it != ch->_clients.end(); it++){
        if((*it)->get_socket() == user->get_socket()){
            return 1;
        }
    }
    return 0;
}
void KICK_command(client_1 *user,std::vector<channel*> ch, std::vector<std::string> tokens)
{
    (void)ch;
    std::string name = tokens[1];
    remove_char(name, '\n');
    channel *channel = search_for_channel(name);
     
    if(channel == NULL){
        send(user->get_socket(), "Channel not found\n", strlen("Channel not found\n"), 0);
        return;
    }
    //if the user is not in the channel
    if(check_if_user_in_channel(user, channel) == 0){
        send(user->get_socket(), "You are not in the channel\n", strlen("You are not in the channel\n"), 0);
        return;
    }
    
    if(chacke_if_addmin(user, channel) == 0){
        send(user->get_socket(), "You are not an admin\n", strlen("You are not an admin\n"), 0);
        return;
    }
    else {//kick the user from the channel
        std::string name = tokens[2];
        remove_char(name, '\n');
        std::vector<client_1*>::iterator it;
        for (it = channel->_clients.begin(); it != channel->_clients.end(); it++){
            if((*it)->get_name() == name){
                if((*it)->get_admin() == 1 && user->get_super_admin() == 0){
                    send(user->get_socket(), "You can't kick an admin\n", strlen("You can't kick an admin\n"), 0);
                    return;
                }
                user_get_kicked((*it)->get_socket(), channel->get_name());
                std::string message2 = (*it)->get_name() + " has been kicked from the channel -->" + channel->get_name() + "\n";
                channel->_clients.erase(std::remove(channel->_clients.begin(), channel->_clients.end(), (*it)), channel->_clients.end());
                channel->remove_admin_by_name((*it)->get_name());
                broadcast_message(channel, user, message2);
                return;
            }
        }
        send(user->get_socket(), "User not found\n", strlen("User not found\n"), 0);
    }
}
