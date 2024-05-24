 #include "../../includes/Global.hpp"
 
std::vector<channel*> all_channels;
std::vector<client_1*> all_clients;
lst *lst_head = NULL;
void lst_add_back(void *ptr)
{
    lst *new_lst = new lst;
    new_lst->ptr = ptr;
    new_lst->next = NULL;
    if (lst_head == NULL)
        lst_head = new_lst;
    else
    {
        lst *tmp = lst_head;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new_lst;
    }
}