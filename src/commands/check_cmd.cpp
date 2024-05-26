#include "../../includes/client.hpp"

void	client::check_cmd(int fd, std::string input)
{
	int index = get_index_client(fd);
	if (index == -1)
	{
		putstr_fd(fd, "You are not connected to the server, use /bot for more information\n");
		return ;
	}
	size_t start = 0;
	std::string param;
	if (param_count(input) > 1)
		param = get_param(input, 2, 1);
	size_t end = start;
	if (input[start] == '/')
		start++;
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
	if (start == 0)
		end++;
	if ((input.substr(start, end - 1) == "nick" || input.substr(start, end - 1) == "NICK") && param_count(input) == 2) ///// ----------- /nick <nickname>
		nick(fd, index, param);
	else if ((input.substr(start, end - 1) == "nick" || input.substr(start, end - 1) == "NICK") && param_count(input) == 1) ////// --------- /nick
		putstr_fd(fd, get_nickname(index) + "\n");
	else if (input.substr(start, end - 1) == "user" && param_count(input) == 2) ////// --------- /user <username>
		user(fd, index, param);
	else if (input.substr(start, end - 1) == "user" && param_count(input) == 1) ////// --------- /user
		putstr_fd(fd, get_username(index) + "\n");
	else if (input.substr(start, end - 1) == "realname" && param_count(input) == 2) ////// --------- /realname <realname>
		realname(fd, index, param);
	else if (input.substr(start, end - 1) == "realname" && param_count(input) == 1) ////// --------- /realname
		putstr_fd(fd, get_realname(index) + "\n");
	else if (input.substr(start, end - 1) == "quit" && param_count(input) == 1) ////// --------- /quit
		quit(fd, index);
	else if (input.substr(start, end - 1) == "bot" && param_count(input) == 1) ////// --------- /bot
		help(fd);
	else if (input.substr(start, end - 1) == "connect")
		putstr_fd(fd, "IRC: You are already connected to the server\n");
	else if (input.substr(start, end - 1) == "PRIVMSG" && param_count(input) >= 3) ////// --------- PRIVMSG <nickname> <message>
		PRIVMSG(fd, index, param, input);
	else if((input.substr(start, end - 1) == "FILEMSG" && param_count(input) >= 3)) ////// --------- FILEMSG <nickname> <file>
		FILEMSG(fd, index, param, input);
	else if (get_flag(get_index_client(fd)) > 0)
		file_confirmation(fd, index, input, start, end);
	else
		check_other_commands(input, all_clients[index]);
}

void copy_data_to_client_1(client_1 *user, int fd, std::string name, std::string nick)
{
	user->set_socket(fd);
	user->set_name(name);
	user->set_nick(nick);
	user->set_admin(0);
	for(size_t i = 0; i < all_clients.size(); i++)
	{
		if(all_clients[i] == NULL){
			all_clients[i] = user;
			return ;
		}
	}
	all_clients.push_back(user);
}

int client::check_input(std::string input, int fd, Server &sev)
{
	// parse input and check if it is a valid command to connecte to the server-------------------------------------------------- connect
	size_t start = 0;
	size_t end = start;

	if (input[start] == '/')
		start = 1;
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
	if (start == 0)
		end++;
	if (input.substr(start, end - 1) == "bot" && param_count(input) == 1) /// ----------------- /bot
	{
		help_to_connect(fd);
		return (1);
	}
	if (input.substr(start, end - 1) == "connect" && check_ip_port(fd , input.substr(end, input.length() - end), sev) == 0 && param_count(input) == 4)
	{
		if (check_max_clients() == -1)
		{
			putstr_fd(fd, "IRC: The server is full, please try again later\n");
			return (1);
		}
		client_1 *user = new client_1();
		lst_add_back(user);
		std::cout << "A new client is connected" << std::endl;
		set_client(fd);
		welcome_message(fd);
		putstr_fd(fd, "IRC: Please enter a command to continue...\n");
		copy_data_to_client_1(user, fd, get_username(get_index_client(fd)), get_nickname(get_index_client(fd)));
		return (0);
	}
	else if (input.substr(start, end - 1) == "exit" && param_count(input) == 1) ////// --------- /exit 
	{
		putstr_fd(fd, "IRC: You have exited from the server\n");
		close(fd);
	}
	else
	{
		putstr_fd(fd, "IRC: You are not connected to the server.\n");
        putstr_fd(fd, "IRC: Please connect to the server first. Use /bot for more information.\n");
		return (1);
	}
	return (0);
}


int	client::send_file(int fd, int fd_recv, std::string file)
{
    std::ifstream ifs(file.c_str());
    std::string content;

	if (!ifs)
	{
		putstr_fd(fd, "IRC: Error opening file\n");
		return (1);
	}
    std::string line;
    while (std::getline(ifs, line))
    {
        content += line;
        content += '\n';
    }
	ifs.close();
	if (content.empty())
	{
		putstr_fd(fd, "IRC: The file is empty\n");
		return (1);
	}
	if (fd == fd_recv)
	{
		putstr_fd(fd, "IRC: You have received a file contnet sent by you !!\n");
		putstr_fd(fd_recv, content);
		return (2);
	}
	set_save(content, get_index_client(fd_recv));
	set_flag(fd, get_index_client(fd_recv));
    return (0);
}

int	client::check_max_clients()
{
	int i = 0;
	while (i < MAX_CLIENTS)
	{
		if (_fd[i] == 0 || _fd[i] == -1)
			return (i);
		i++;
	}
	return (-1);
}