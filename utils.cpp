#include "client.hpp"

int	skep_space(std::string input, size_t start)
{
	while (input.length() > 0 && input[start] == ' ' && start < input.length())
		start++;
	return (start);
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