#include "../../includes/server.hpp"

bool is_digits(const std::string &str)
{
    return (str.find_first_not_of("0123456789") == std::string::npos);
}

void    check_port(std::string av)
{
    int port;

    if (is_digits(av) == 0)
    {
        std::cout << "Please use numbers only" << std::endl;
        std::exit(1);
    }
    port = std::atoi(av.c_str());
    if (port > 0 && port < 1024)
    {
        std::cout << av << " This port is private" << std::endl;
        std::exit(1);
    }
    if (port < 0 || port > 65536)
    {
        std::cout << av << " This port is invalid" << std::endl;
        std::exit(1);
    }
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Error in arguments" << std::endl;
        return (1);
    }
    check_port(av[1]);
    Server sev(av[1], av[2]);
    sev.InitServer(sev);
}
