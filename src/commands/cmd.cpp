#include "../../includes/client.hpp"


void    client::nick(int fd, int index, std::string param)
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

void    client::user(int fd, int index, std::string param)
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
			update_user_command(get_str_no_space(param), get_username(index));//
			set_username(get_str_no_space(param), index);
			putstr_fd(fd, "IRC: Your username has been set successfully to: ");
			putstr_fd(fd, get_username(index));
			putstr_fd(fd, "\n");
		}
}

void    client::realname(int fd, int index, std::string param)
{
    if (check_if_aviable(param, _realname) == 2)
		putstr_fd(fd, "IRC: Invalid realname, please enter a valid one (contain only letters and numbers)\n");
	else
	{
		update_real_name_command(get_str_no_space(param), get_nickname(index));//
		set_realname(get_str_no_space(param), index);
		putstr_fd(fd, "IRC: Your realname has been set successfully to: ");
		putstr_fd(fd, get_realname(index));
		putstr_fd(fd, "\n");
	}
}

void    client::quit(int fd, int index)
{
    goodbye_message(fd);
	std::cout << "A client has been disconnected" << std::endl;
	delete_client(index);
	delete_user_from_channel(fd);
	close(fd);
}

void    client::PRIVMSG(int fd, int index, std::string param, std::string input)
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

void    client::FILEMSG(int fd, int index, std::string param, std::string input)
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
				putstr_fd(get_fd_by_nickname(param), "IRC: Ps --> every other input will be considered as a refusal\n");
			}
		}
}

void    client::file_confirmation(int fd, int index, std::string input, size_t start, size_t end)
{
    if (input.substr(start, end - 1) == "Yes")
		{
			putstr_fd(fd, "IRC: You have accepted to receive the file :\n");
			putstr_fd(fd, _save[get_index_client(fd)]);
			_save[get_index_client(fd)].clear();
			set_flag(-1, get_index_client(fd));
		}
		else if (input.substr(start, end - 1) == "No")
		{
			putstr_fd(fd, "IRC: You have refused to receive the file\n");
			_save[index].clear();
			set_flag(-1, get_index_client(fd));
		}
		else
		{
			putstr_fd(fd, "IRC: You have refused to receive the file\n");
			_save[get_index_client(fd)].clear();
			set_flag(-1, get_index_client(fd));
			putstr_fd(fd, "IRC: Invalid command, use /bot for more information\n");
		}
}
