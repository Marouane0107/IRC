#include "../../includes/client.hpp"


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
		if (input[start] == '\0' || input[start] == '\n')
			break;
		while(input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
			end++;
		if (i == 0)
		{
			if(get_param(input, 1, 1) != ip)
			{
				putstr_fd(fd, "Invalid ip / \n");
				return (1);
			}
		}
		else if (i == 1)
		{
			if(get_param(input, 2, 1) != port)
			{
				putstr_fd(fd, "Invalid port / \n");
				return (1);
			}
		}
		else if (i == 2)
		{
			if (get_param(input, 3, 1) != pass)
			{
				putstr_fd(fd, "Invalid password / \n");
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

std::string ft_itos(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

int	skep_space(std::string input, size_t start)
{
	while (input.length() > 0 && input[start] == ' ' && start < input.length())
		start++;
	return (start);
}

int get_len_no_space(std::string input, size_t start)
{
	size_t	len = 0;

	while (start < input.length() && input[start] != '\0' && input[start] != '\n' && input[start] != ' ' && input[start] != '\r')
	{
		len++;
		start++;
	}
	{
		if (input[start] != ' ' && input[start] != '\n')
			len++;
		start++;
	}
	return (len);
}

void    putstr_fd(int fd, std::string str)
{
	int i = 0;

	if (str.empty())
		return ;
	while (str[i] != '\0')
	{
		send(fd, &str[i], 1, 0);
		i++;
	}
}

std::string	get_str_no_space(std::string input)
{
	std::string name;
	int start = skep_space(input, 0);
	name = input.substr(start, get_len_no_space(input, start));
	return (name);
}

int	param_count(std::string input)
{
	size_t start = 0;
	size_t end = 0;
	int count = 0;

	while (end < input.length() && input[end] != '\0' && input[end] != '\n')
	{
		start = skep_space(input, start);
		if (input[start] == '\0' || input[start] == '\n')
			return (count);
		end = start;
		while(input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
			end++;
		start = end;
		count++;
	}
	return (count);
}

std::string	get_param(std::string input, int param_number, int f)
{
	int i = 1;
	size_t start = 0;
	size_t end = 0;
	
	while (end < input.length() && input[end] != '\0' && input[end] != '\n')
	{
		start = skep_space(input, start);
		end = start;
		while(input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
			end++;
		if (i == param_number && f == 0)
		{
			end = start;
			while (input[end] != '\0' && end < input.length())
				end++;
			return (input.substr(start, end - start));
		}
		if (i == param_number && f == 1)
			return (input.substr(start, end - start));
		start = end;
		i++;
	}
	return ("");
}

std::string	one_by_one(std::string input, size_t *start)
{
	size_t end = *start;
	std::string name;

	while(input.length() > 0 && input[end] != '\0' && input[end] != ' ' && input[end] != '\n' && end < input.length())
		end++;
	name = input.substr(*start, end - *start);
	*start = end;
	return (name);
}

