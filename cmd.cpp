#include "client.hpp"
#include "server.hpp"


void	client::check_cmd(int fd, std::string input)
{
	int index = get_index_client(fd);
	if (index == -1)
	{
		putstr_fd(fd, "You are not connected to the server, use /bot for more information\n");
		return ;
	}
	size_t start = skep_space(input, 0);
	size_t end = start;
	std::string param;
	if (param_count(input) > 1)
		param = get_param(input, 2);
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
	if (get_str_no_space(input) == "/nick" && param_count(input) == 2) ///// ----------- /nick <nickname> ------------------------
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
			set_nickname(get_str_no_space(param), index);
			putstr_fd(fd, "IRC: Your nickname has been set successfully to: ");
			putstr_fd(fd, get_nickname(index));
			putstr_fd(fd, "\n");
		}
	}
	else if (get_str_no_space(input) == "/nick" && param_count(input) == 1) ////// --------- /nick ---------------------
		putstr_fd(fd, get_nickname(index) + "\n");
	else if (get_str_no_space(input) == "/user" && param_count(input) == 2) ////// --------- /user <username> -------------------
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
			set_username(get_str_no_space(param), index);
			putstr_fd(fd, "IRC: Your username has been set successfully to: ");
			putstr_fd(fd, get_username(index));
			putstr_fd(fd, "\n");
		}
	}
	else if (get_str_no_space(input) == "/user" && param_count(input) == 1) ////// --------- /user --------------------
		putstr_fd(fd, get_username(index) + "\n");
	else if (get_str_no_space(input) == "/realname" && param_count(input) == 2) ////// --------- /realname <realname> -------------------
	{
		if (check_if_aviable(param, _realname) == 2)
			putstr_fd(fd, "IRC: Invalid realname, please enter a valid one (contain only letters and numbers)\n");
		else
		{
			set_realname(get_str_no_space(param), index);
			putstr_fd(fd, "IRC: Your realname has been set successfully to: ");
			putstr_fd(fd, get_realname(index));
			putstr_fd(fd, "\n");
		}
	}
	else if (get_str_no_space(input) == "/realname" && param_count(input) == 1) ////// --------- /realname --------------------
		putstr_fd(fd, get_realname(index) + "\n");
	else if (get_str_no_space(input) == "/quit" && param_count(input) == 1) ////// --------- /quit --------------------
	{
		goodbye_message(fd);
		std::cout << "A client has been disconnected" << std::endl;
		delete_client(index);
		close(fd);
	}
	else if (get_str_no_space(input) == "/bot" && param_count(input) == 1) ////// --------- /bot  help --------------------
		help(fd);
	else if (get_str_no_space(input) == "/connect")
		putstr_fd(fd, "IRC: You are already connected to the server\n");
	else if (get_str_no_space(input) == "/PRIVMSG" && param_count(input) >= 3) ////// --------- PRIVMSG <nickname> <message> -------
	{
		if (get_fd_by_nickname(param) == -1)
			putstr_fd(fd, "IRC: This user is not exist, please enter a valid nickname\n");
		else
		{
			putstr_fd(get_fd_by_nickname(param), "IRC: You have received a private message from ");
			putstr_fd(get_fd_by_nickname(param) ,get_nickname(index) + ": \n----> ");
			putstr_fd(get_fd_by_nickname(param), get_param(input, 3));
			putstr_fd(fd, "IRC: You have sent a private message to ");
			putstr_fd(fd, get_nickname(get_index_client(get_fd_by_nickname(param))));
			putstr_fd(fd, "\n");
		}
	}
	else
		putstr_fd(fd, "IRC: Invalid command, use /bot for more information\n");
}

int client::check_input(std::string input, int fd, Server &sev)
{
	// parse input and check if it is a valid command to connecte to the server-------------------------------------------------- connect
	size_t start = skep_space(input, 0);
	size_t end = start;
	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
    putstr_fd(fd, get_str_no_space(input) + "\n");//-----------------------------------------------------------------------------------------------------------debug
	if (get_str_no_space(input) == "/bot" && param_count(input) == 1) /// ----------------- /bot --------------------- help message before connecting
	{
		help_to_connect(fd);
		return (1);
	}
	if (get_str_no_space(input) == "/connect" && check_ip_port(fd , input.substr(end, input.length() - end -1), sev) == 0 && param_count(input) == 4)
	{
		welcome_message(fd);
		std::cout << "A new client is connected" << std::endl;
		set_client(fd);
		putstr_fd(fd, "IRC: Please enter a command to continue...\n");
		return (0);
	}
	else
	{
		putstr_fd(fd, "IRC: Connect to the server first, use /bot for more information \n");
		return (1);
	}
	return (0);
}
