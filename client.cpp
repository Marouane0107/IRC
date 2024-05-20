#include "server.hpp"
#include "client.hpp"


client::client()
{
	_port = "0";
	_address = "127.0.0.1";

}

client::~client(){	
}

std::string client::get_nickname(int index)
{
	return (_nickname[index]);
}

int client::get_fd(int index)
{
	return (_fd[index]);
}

std::string client::get_username(int index)
{
	return (_username[index]);
}

std::string client::get_realname(int index)
{
	return (_realname[index]);
}

std::string client::get_address()
{
	return (_address);
}

std::string client::get_port()
{
	return (_port);
}

int client::get_socketfile()
{
	return (socketfile);
}

void	client::set_username(std::string username, int index)
{
	_username[index] = username;
}

void	client::set_nickname(std::string nickname, int index)
{
	_nickname[index] = nickname;
}


void	client::set_realname(std::string realname, int index)
{
	_realname[index] = realname;
}

void	client::set_address(std::string address)
{
	_address = address;
}

void	client::set_port(std::string port)
{
	_port = port;
}

void	client::set_client(int fd)
{
	int index = 0;
	while (index < MAX_CLIENTS)
	{
		if (_fd[index] <= 0 )
		{
			_fd[index] = fd;
			_username[index] = "user_";
			_nickname[index] = "usernick_";
			_realname[index] = "Unknown";
			return ;
		}
		index++;
	}
}

int check_ip_port(int fd, std::string input, Server &sev)
{
	size_t i = 0;
	size_t start = 0;
	size_t end = 0;
	std::string ip = sev.get_address();
	std::string port = sev.get_port();
	std::string pass = sev.get_pass();
	while (end < input.length() && input[end] != '\0' && input[end] != '\n')
	{
		start = skep_space(input, start);
		end = start;
		while(input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
			end++;
		if (i == 0)
		{
			if(input.substr(start, ip.length()) != ip)
			{
				send(fd, "Invalid ip / \n", 15, 0);
				return (1);
			}
		}
		if (i == 1)
		{
			if(input.substr(start, port.length()) != port)
			{
				send(fd, "Invalid port / \n", 17, 0);
				return (1);
			}
		}
		if (i == 2)
		{
			if (input.substr(start, pass.length()) != pass)
			{
				send(fd, "Invalid password / \n", 21, 0);
				return (1);
			}
		}
		start = end;
		i++;
	}
	start = skep_space(input, start);
	if (i != 3 && start < input.length())
		return (1);
	return (0);
}

int client::get_index_client(int fd)
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (fd == _fd[i])
			return (i);
	}
	return (-1);
}

int client::get_fd_by_nickname(std::string nickname)
{
	nickname = get_str_no_space(nickname);
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (nickname == _nickname[i])
			return (_fd[i]);
	}
	return (-1);
}

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
	if (input.substr(start, end) == "/nick" && param_count(input) == 2) ///// ----------- /nick <nickname> ------------------------
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
	else if (input.substr(start, end) == "/nick" && param_count(input) == 1) ////// --------- /nick ---------------------
		putstr_fd(fd, get_nickname(index) + "\n");
	else if (input.substr(start, end) == "/user" && param_count(input) == 2) ////// --------- /user <username> -------------------
	{
		if (check_if_aviable(param, _username) != 0)
		{
			if (check_if_aviable(param, _username) == 2)
				putstr_fd(fd, "IRC: Invalid username, please enter a valid one (contain only letters and numbers)\n");
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
	else if (input.substr(start, end) == "/user" && param_count(input) == 1) ////// --------- /user --------------------
		putstr_fd(fd, get_username(index) + "\n");
	else if (input.substr(start, end) == "/realname" && param_count(input) == 2) ////// --------- /realname <realname> -------------------
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
	else if (input.substr(start, end) == "/realname" && param_count(input) == 1) ////// --------- /realname --------------------
		putstr_fd(fd, get_realname(index) + "\n");
	else if (input.substr(start, end) == "/quit" && param_count(input) == 1) ////// --------- /quit --------------------
	{
		goodbye_message(fd);
		std::cout << "A client has been disconnected" << std::endl;
		delete_client(index);
		close(fd);
	}
	else if (input.substr(start, end) == "/bot" && param_count(input) == 1) ////// --------- /bot  help --------------------
		help(fd);
	else if (input.substr(start, end) == "/connect")
		putstr_fd(fd, "IRC: You are already connected to the server\n");
	else if (input.substr(start, end) == "/PRIVMSG" && param_count(input) >= 3) ////// --------- PRIVMSG <nickname> <message> -------
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
	if (input.substr(start, end) == "/bot" && param_count(input) == 1) /// ----------------- /bot --------------------- help message before connecting
	{
		help_to_connect(fd);
		return (1);
	}
	if (input.substr(start, end) == "/connect" && check_ip_port(fd , input.substr(end, input.length() - end -1), sev) == 0 && param_count(input) == 4)
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

void	client::delete_client(int index)
{
	_fd[index] = -1;
	_nickname[index] = "";
	_username[index] = "";
	_realname[index] = "";
}

void	client::init_all(Server &sev)
{
	socketfile = sev.get_socketfile();
	memset(_fd, -2, sizeof(_fd));
	memset(_nickname, 0, sizeof(_nickname));
	memset(_username, 0, sizeof(_username));
	memset(_realname, 0, sizeof(_realname));
}

int		client::check_if_aviable(std::string input, std::string* list)
{
	size_t i = 0;
	std::string name;
	size_t start = skep_space(input, 0);
	size_t end = start;
	while(input.length() > 0 && std::isalpha(input[end]) && end < input.length())
		end++;
	if (input[end] != '\0' && input[end] != ' ' && input[end] != '\n')
		return (2);
	name = get_str_no_space(input);
	if (name.empty())
		return (2);
	while (i < MAX_CLIENTS)
	{
		if (name == list[i] && _fd[i] > 0)
			return (1);
		i++;
	}
	return (0);
}
