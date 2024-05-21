#include "client.hpp"


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

