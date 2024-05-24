#include "../../includes/client.hpp"

void	client::welcome_message(int fd) {
	putstr_fd(fd, "+---------------| Assistance Bot is Here |---------------+\n");
	putstr_fd(fd, "|-------------------| Welcome to IRC |-------------------|\n");
	putstr_fd(fd, "|                                                        |\n");
	putstr_fd(fd, "| Welcome! You've successfully connected to the IRC!     |\n");
	putstr_fd(fd, "|                                                        |\n");
	putstr_fd(fd, "| Youre default information:                             |\n");
	putstr_fd(fd, "|                                                        |\n");
	putstr_fd(fd, "| Username: " + get_username(get_index_client(fd)) + "   \n");
	putstr_fd(fd, "| Nickname: " + get_nickname(get_index_client(fd)) + "   \n");
	putstr_fd(fd, "| Realname: " + get_realname(get_index_client(fd)) + "   \n");
	putstr_fd(fd, "|                                                        |\n");
	putstr_fd(fd, "| The information can be changed using the following:    |\n");
	putstr_fd(fd, "| /nick <nickname> - Change your nickname                |\n");
	putstr_fd(fd, "| /user <username> - Change your username                |\n");
	putstr_fd(fd, "| /realname <realname> - Change your real name           |\n");
	putstr_fd(fd, "|                                                        |\n");
	putstr_fd(fd, "| Enjoy chatting with others. For help, type /bot.       |\n");
	putstr_fd(fd, "|                                                        |\n");
	putstr_fd(fd, "+--------------------------------------------------------+\n");
}

void	help_to_connect(int fd)
{
	putstr_fd(fd, "+-----------| Assistance Bot at your service |---------+\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "| To connect to the server, use the following command: |\n");
	putstr_fd(fd, "| /connect <address> <port> <password>                 |\n");
	putstr_fd(fd, "| Example: /connect 127.0.0.1 8080 mypassword          |\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "+------------------------------------------------------+\n");
}

void    help(int fd)
{
	putstr_fd(fd, "+---------------| Assistance Bot at your service |-------------------+\n");
	putstr_fd(fd, "|                                                                    |\n");
	putstr_fd(fd, "|           List of Available Commands:                              |\n");
	putstr_fd(fd, "| /FILEMSG <nickname> <file> - Send a file to a user                 |\n");
	putstr_fd(fd, "| /nick <nickname> - Change your nickname                            |\n");
	putstr_fd(fd, "| /nick - See your nickname                                          |\n");
	putstr_fd(fd, "| /user <username> - Change your username                            |\n");
	putstr_fd(fd, "| /user - See your username                                          |\n");
	putstr_fd(fd, "| /realname <realname> - Change your real name                       |\n");
	putstr_fd(fd, "| /realname - See your real name                                     |\n");
	putstr_fd(fd, "| /connect <address> <port> <password> - Connect to the server       |\n");
	putstr_fd(fd, "| /PRIVMSG <nickname> <message> - Send a private message             |\n");
	putstr_fd(fd, "| /quit - Disconnect from the server                                 |\n");
	putstr_fd(fd, "|--------------------------------------------------------------------|\n");
	putstr_fd(fd, "| /bot - Display this help message ;)                                |\n");
	putstr_fd(fd, "|--------------------------------------------------------------------|\n");
	putstr_fd(fd, "| /join #<channel> - Join a channel or create a new one if not exist |\n");
	putstr_fd(fd, "| /KICK <channel> <user> - Eject a client from the channel           |\n");
	putstr_fd(fd, "| /LEAVE <channel> - Leave a channel                                 |\n");
	putstr_fd(fd, "| /LIST - List all available channels                                |\n");
	putstr_fd(fd, "| /INVITE <channel> <user> - Invite a client to a channel            |\n");
	putstr_fd(fd, "| /TOPIC <channel> [<topic>] - Change or view the channel topic      |\n");
	putstr_fd(fd, "| /MODE <channel> <mode> - Change the channels mode                  |\n");
	putstr_fd(fd, "|   i: Set/remove Invite-only channel                                |\n");
	putstr_fd(fd, "|   t: Set/remove the restrictions of the TOPIC command to channel   |\n");
	putstr_fd(fd, "|      operators                                                     |\n");
	putstr_fd(fd, "|   k: Set/remove the channel key (password)                         |\n");
	putstr_fd(fd, "|   o: Give/take channel operator privilege                          |\n");
	putstr_fd(fd, "|   l: Set/remove the user limit to channel                          |\n");
	putstr_fd(fd, "|                                                                    |\n");
	putstr_fd(fd, "+--------------------------------------------------------------------+\n");
}

void    goodbye_message(int fd) {
	putstr_fd(fd, "+---------------| Assistance Bot is Here |-------------+\n");
	putstr_fd(fd, "+------------------| Goodbye from IRC |----------------+\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "| You have disconnected from the IRC.                  |\n");
	putstr_fd(fd, "| Thank you for visiting. Have a great day!            |\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "+------------------------------------------------------+\n");
}

void    welcome_user_channel(int fd, std::string channel)
{
	putstr_fd(fd, "+---------------| Assistance Bot is Here |---------------+\n");
	putstr_fd(fd, "|                                                        |\n");
	putstr_fd(fd, "| Welcome! You've successfully joined the channel:       |\n");
	putstr_fd(fd, "|       ---------> " + channel + " <-------------        \n");
	putstr_fd(fd, "| Enjoy chatting with others. For help, type /bot.       |\n");
	putstr_fd(fd, "|                                                        |\n");
	putstr_fd(fd, "+--------------------------------------------------------+\n");
}

void    goodbye_user_channel(int fd, std::string channel)
{
	putstr_fd(fd, "+---------------| Assistance Bot is Here |-------------+\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "| You have left the channel:                           |\n");
	putstr_fd(fd, "|                  ----> " + channel + " <----         \n");
	putstr_fd(fd, "| Thank you for visiting. Have a great day!            |\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "+------------------------------------------------------+\n");
}

void	annonce_user_channel(int fd, std::string channel, std::string nickname)
{
	putstr_fd(fd, "+---------------| Assistance Bot is Here |-------------+\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "| Say hello to our new friend!                         |\n");
	putstr_fd(fd, "| " + nickname + " has joined the channel:             \n");
	putstr_fd(fd, "|                  ----> " + channel + " <----         \n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "+------------------------------------------------------+\n");
}

void	annonce_user_channel_leave(int fd, std::string channel, std::string nickname)
{
	putstr_fd(fd, "+---------------| Assistance Bot is Here |-------------+\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "| Goodbye to our friend!                               |\n");
	putstr_fd(fd, "| " + nickname + " has left the channel:               \n");
	putstr_fd(fd, "|                  ----> " + channel + " <----         \n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "+------------------------------------------------------+\n");
}

void	user_get_kicked(int fd, std::string channel)
{
	putstr_fd(fd, "+---------------| Assistance Bot is Here |-------------+\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "| I'm sorry to inform you that You have been kicked    |\n");
	putstr_fd(fd, "| from the channel:                                    |\n");
	putstr_fd(fd, "|                  ----> " + channel + " <----         \n");
	putstr_fd(fd, "| Reason: Because the admin doesn't like you :(        |\n");
	putstr_fd(fd, "|                                                      |\n");
	putstr_fd(fd, "+------------------------------------------------------+\n");
}
