#include "server.hpp"
#include "client.hpp"

void    check_port(std::string av)
{
    int port;

    port = std::atoi(av.c_str());
    if (port > 0 && port < 1024)
    {
        std::cout << av << " this port is private" << std::endl;
        std::exit(1);
    }
    if (port < 0 || port > 65536)
    {
        std::cout << av << " you can't use this port" << std::endl;
        std::exit(1);
    }
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "error in the argv" << std::endl;
        return 1;
    }
    check_port(av[1]);
    Server sev(av[1], av[2]);
    sev.InitServer(sev);
}