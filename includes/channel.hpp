#ifndef HOW_HPP
#define HOW_HPP

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "MODE.hpp"
#include "client.hpp"
#include <vector>
#include <algorithm>
//IRC CHANNEL CLASS
// const int MAX_CLIENTS = 10;

struct clien_info
{
    int socket;
    char cmd[1024];
    char name[1024];
    char nick[1024];
    std::vector<std::string> tokens;
    clien_info()
    {
        socket = -1;
        memset(cmd, 0, sizeof(cmd));
        memset(name, 0, sizeof(name));
        memset(nick, 0, sizeof(nick));
    };
};
class channel;
class client_1
{
    public:
        ~client_1();
        client_1();
        int get_socket();
        std::string get_name();
        std::string get_nick();
        int get_admin();
        void set_socket(int socket);
        void set_name(std::string name);
        void set_nick(std::string nick);
        void set_admin(int admin);
        void set_real_name(std::string real_name);
        void set_super_admin(int super_admin);
        int get_super_admin();
        std::string get_real_name();
        // channel *ptr_channel;
        std::vector<channel*> _channels;// the user may be in more than one channel
    private:
        int _socket;
        int is_admin;//1 for admin, 0 for znormal user 2 for mini admin
        int super_admin;//1 for super admin, 0 for normal admin
        std::string _name;//user name
        std::string _nick;
        std::string real_name;
};

class channel
{
    public:
        channel(std::string name);
        void add_admine_by_name(std::string name);
        void remove_admin_by_name(std::string name);
        ~channel();
        std::string get_name();
        void add_client(client_1 *cl);
        std::vector<client_1*> _clients;
        void add_admin(client_1 *cl);
        void remove_admin(client_1 *cl);
        void set_topic(std::vector<std::string> topic);
        void set_password(std::string password);
        void set_channel_mode(int mode);
        void set_si_password_required(int si_password_required);
        void set_name(std::string name);
        std::string get_topic();
        std::string get_password();
        int get_channel_mode();
        int get_si_password_required();
        int check_if_admin(client_1 *cl);
        void set_topic_changeable(int topic_changeable);
        int get_topic_changeable();
        std::vector<client_1*> get_admins();
        int get_is_max_clients_required();
        size_t get_number_of_clients();
        void set_is_max_clients_required(int is_max_clients_required);
        void set_number_of_clients(size_t number_of_clients);
    private:
        int is_max_clients_required;//0 for no, 1 for yes
        int number_of_clients; 
        std::string _name;
        int channel_mode;// 0 for invite only, 1 for public
        std::string _password;
        std::vector<client_1*> _admins;// ones who can kick, invite and change topic
        std::string _topic;
        int topic_changeable;//0 for no, 1 for yes
        int si_password_required;//0 for no, 1 for yes
        // int socket;
        
};

// the user may be an admin for more than one channel
//golobal vector of channels
// MACROS MESSAGES
#define TOPIC "What Is the topci of the channle :" 
#define NAME_MSG "please your name "
#define NICK_NAME_MSG "you nickname :"
#define CREAT_CHANNEL "channel created: and you've joined "
#define JOIN_CHANNEL "channel found, joining the channel ..."
#define JOINED_CHANNEL "joined channel ... done "
#define WELCOME_CHANNEL "\n[NEW USER JOINED THE CHANNEL :]\n"
////////////////////////////////////////////////////////////
void collect_data_from_user(clien_info *cl);
void topic_cmd(std::vector<std::string> tokens, client_1 *user);
void check_other_commands(std::string input, client_1 *user);
void invite_cmd(std::vector<std::string> tokens, client_1 *user);
void remove_char(std::string &str, char ch);
void update_user_command(std::string new_user, std::string old_user);
void send_to_all_clients_in_channel(channel *ch, client_1 *user);
void sending_msg(std::vector<std::string> tokens, client_1 *user);
void delete_user_from_channel(int fd);
void list_command(int fd);
std::vector<std::string> split(const std::string& str, char delimiter);
#endif


