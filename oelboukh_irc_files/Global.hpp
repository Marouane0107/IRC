#ifndef G_VAR_HPP
#define G_VAR_HPP

#include <iostream>
#include <vector>
class client_1;
class channel;

extern std::vector<channel*> all_channels;
extern std::vector<client_1*> all_clients;
struct lst
{
    void *ptr;
    lst *next;
};

extern lst *lst_head;
void lst_add_back(void *ptr);

 #endif