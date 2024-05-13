#include "server.hpp"


int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "error in the argv" << std::endl;
        return 1;
    }

    Server sev(av[1], av[2]);

    sev.initsever(sev);
}