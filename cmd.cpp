#include "client.hpp"
#include "server.hpp"
#include "oelboukh_irc_files/channel.hpp"
#include "oelboukh_irc_files/update.hpp"
#include "oelboukh_irc_files/Global.hpp"
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
	if ((input.substr(start, end - 1) == "nick" || input.substr(start, end - 1) == "NICK") && param_count(input) == 2) ///// ----------- /nick <nickname> ------------------------
	{
		if (check_if_aviable(param, _nickname) != 0)
		{
			if (check_if_aviable(param, _nickname) == 2)
				putstr_fd(fd, "IRC: Invalid nickname, please enter a valid one (contain only letters and numbers)\n");
			else
				putstr_fd(fd, "IRC: This nickname is already taken, please choose another one\n");
		}
		else
		{
			update_nick_command(get_str_no_space(param), get_nickname(index));//oelboukh
			set_nickname(get_str_no_space(param), index);
			putstr_fd(fd, "IRC: Your nickname has been set successfully to: ");
			putstr_fd(fd, get_nickname(index));
			putstr_fd(fd, "\n");
		}
	}
	else if ((input.substr(start, end - 1) == "nick" || input.substr(start, end - 1) == "NICK") && param_count(input) == 1) ////// --------- /nick ---------------------
		putstr_fd(fd, get_nickname(index) + "\n");
	else if (input.substr(start, end - 1) == "user" && param_count(input) == 2) ////// --------- /user <username> -------------------
	{
		if (check_if_aviable(param, _username) != 0)
		{
			if (check_if_aviable(param, _username) == 2)
				putstr_fd(fd, "IRC: Invalid username, please enter a valid one (contain only letters)\n");
			else
				putstr_fd(fd, "IRC: This username is already taken, please choose another one\n");
		}
		else
		{
			update_user_command(get_str_no_space(param), get_username(index));//oelboukh line 2 for user name
			set_username(get_str_no_space(param), index);
			putstr_fd(fd, "IRC: Your username has been set successfully to: ");
			putstr_fd(fd, get_username(index));
			putstr_fd(fd, "\n");
		}
	}
	else if (input.substr(start, end - 1) == "user" && param_count(input) == 1) ////// --------- /user --------------------
		putstr_fd(fd, get_username(index) + "\n");
	else if (input.substr(start, end - 1) == "realname" && param_count(input) == 2) ////// --------- /realname <realname> -------------------
	{
		if (check_if_aviable(param, _realname) == 2)
			putstr_fd(fd, "IRC: Invalid realname, please enter a valid one (contain only letters and numbers)\n");
		else
		{
			update_real_name_command(get_str_no_space(param), get_nickname(index));//oelboukh line 3 for real name
			set_realname(get_str_no_space(param), index);
			putstr_fd(fd, "IRC: Your realname has been set successfully to: ");
			putstr_fd(fd, get_realname(index));
			putstr_fd(fd, "\n");
		}
	}
	else if (input.substr(start, end - 1) == "realname" && param_count(input) == 1) ////// --------- /realname --------------------
		putstr_fd(fd, get_realname(index) + "\n");
	else if (input.substr(start, end - 1) == "quit" && param_count(input) == 1) ////// --------- /quit --------------------
	{
		goodbye_message(fd);
		std::cout << "A client has been disconnected" << std::endl;
		delete_client(index);
		close(fd);
	}
	else if (input.substr(start, end - 1) == "bot" && param_count(input) == 1) ////// --------- /bot  help --------------------
		help(fd);
	else if (input.substr(start, end - 1) == "connect")
		putstr_fd(fd, "IRC: You are already connected to the server\n");
	else if (input.substr(start, end - 1) == "PRIVMSG" && param_count(input) >= 3 && param[0] != '#') ////// --------- PRIVMSG <nickname> <message> -------
	{
		if (get_fd_by_nickname(param) == -1)
			putstr_fd(fd, "IRC: This user is not exist, please enter a valid nickname\n");
		else
		{
			if (get_fd_by_nickname(param) == fd)
			{
				putstr_fd(fd, "IRC: You have sent a private message to yourself\n");
				putstr_fd(fd, "IRC: You have received a private message sent by you !!\n");
				putstr_fd(fd, "    -->me: \n");
				putstr_fd(fd, get_param(input, 3, 0));
			}
			else
			{
				putstr_fd(get_fd_by_nickname(param), "IRC: You have received a private message from ");
				putstr_fd(get_fd_by_nickname(param) ,get_nickname(index) + ": \n----> ");
				putstr_fd(get_fd_by_nickname(param), get_param(input, 3, 0));
				putstr_fd(fd, "IRC: You have sent a private message to ");
				putstr_fd(fd, get_nickname(get_index_client(get_fd_by_nickname(param))));
				putstr_fd(fd, "\n");
			}
		}
	}
	else if((input.substr(start, end - 1) == "FILEMSG" && param_count(input) >= 3)) ////// --------- FILEMSG <nickname> <file> ------- need to add accept file !!
	{
		if (get_fd_by_nickname(param) == -1)
			putstr_fd(fd, "IRC: This user is not exist, please enter a valid nickname\n");
		else
		{
			if ((send_file(fd, get_fd_by_nickname(param), get_param(input, 3, 1)) == 0))
			{
				putstr_fd(fd, "IRC: You have sent a file to ");
				putstr_fd(fd, get_nickname(get_index_client(get_fd_by_nickname(param))));
				putstr_fd(fd, "\n");
				putstr_fd(get_fd_by_nickname(param), "IRC: You have received a file from ");
				putstr_fd(get_fd_by_nickname(param), get_nickname(index));
				putstr_fd(get_fd_by_nickname(param), "\n");
				putstr_fd(get_fd_by_nickname(param), "IRC: Please enter 'Yes' to accept the file or 'No' to refuse it\n");
				putstr_fd(get_fd_by_nickname(param), "Ps: enything else will refuse the file\n");
			}
		}
	}
	else if (get_flag(fd) > 0)
	{
		if (input.substr(start, end - 1) == "Yes") // not working properly------------------- need to be fixed
		{
			putstr_fd(fd, "IRC: You have accepted to receive the file :\n");
			putstr_fd(fd, _save[fd]);
			_save[fd].clear();
			set_flag(-1, fd);
		}
		else if (input.substr(start, end - 1) == "No")
		{
			putstr_fd(fd, "IRC: You have refused to receive the file\n");
			_save[index].clear();
			set_flag(-1, fd);
		}
		else
		{
			putstr_fd(fd, "IRC: You have refused to receive the file\n");
			_save[fd].clear();
			set_flag(-1, fd);
			putstr_fd(fd, "IRC: Invalid command, use /bot for more information\n");
		}
	}
	else
		check_other_commands(input, all_clients[index]);//oelboukh line 1
}
void copy_data_to_client_1(client_1 *user, int fd, std::string name, std::string nick)
{
	user->set_socket(fd);
	user->set_name(name);
	user->set_nick(nick);
	user->set_admin(0);
	all_clients.push_back(user);
	std::cout << "New user added to the list of clients" << std::endl;
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
	if (input.substr(start, end - 1) == "bot" && param_count(input) == 1) /// ----------------- /bot --------------------- help message before connecting
	{
		help_to_connect(fd);
		return (1);
	}
	if (input.substr(start, end - 1) == "connect" && check_ip_port(fd , input.substr(end, input.length() - end), sev) == 0 && param_count(input) == 4)
	{
		client_1 *user = new client_1();
		lst_add_back(user);
		welcome_message(fd);
		std::cout << "A new client is connected" << std::endl;
		set_client(fd);
		copy_data_to_client_1(user, fd, get_username(get_index_client(fd)), get_nickname(get_index_client(fd)));//oelboukh
		putstr_fd(fd, "IRC: Please enter a command to continue...\n");
		return (0);
	}
	else if (input.substr(start, end - 1) == "exit" && param_count(input) == 1) ////// --------- /exit --------------------
	{
		putstr_fd(fd, "Disconnected ()\n");
		close(fd);
	}
	else
	{
		putstr_fd(fd, "IRC: Connect to the server first, use /bot for more information \n");
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
	set_save(content, fd_recv);
	set_flag(fd, fd_recv);
    return (0);
}

