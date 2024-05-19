#include "client.hpp"

int	skep_space(std::string input, size_t start)
{
	while (input.length() > 0 && input[start] == ' ' && start < input.length())
		start++;
	return (start);
}

int get_len_no_space(std::string input, size_t start)
{
	size_t	len = 0;

	while (start < input.length() && input[start] != '\0')
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


char*	ft_recv(int fd, char *buffer)
{
	while (true)
	{
		memset(buffer, 0, MAX_BUFFER_SIZE);
		recv(fd, buffer, MAX_BUFFER_SIZE, 0);
		if (strlen(buffer) > 0)
			break;
	}
	return (buffer);
}
