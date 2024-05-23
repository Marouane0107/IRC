#include "channel.hpp"
#include "Global.hpp"
#include "join.hpp"
#include "../client.hpp"

void client_1::set_socket(int socket){
	this->_socket = socket;
}
void client_1::set_real_name(std::string real_name){
	this->real_name = real_name;
}

std::string client_1::get_real_name(){
	return this->real_name;
}

int channel::check_if_admin(client_1 *cl){
	std::vector<client_1*>::iterator it;
	//check if the client is an admin
	for(it = _admins.begin(); it != _admins.end(); it++){
		if((*it)->get_socket() == cl->get_socket()){
			return 1;
		}
	}
	return 0;
}

void channel::add_admine_by_name(std::string name){/// this function is for chose an admin
	std::vector<client_1*>::iterator it;
	//check if the client is an admin
	for(it = _clients.begin(); it != _clients.end(); it++){
		if((*it)->get_name() == name){
			(*it)->set_admin(1);
			_admins.push_back(*it);
			send((*it)->get_socket(), "You are now an admin\n", strlen("You are now an admin\n"), 0);
			return;
		}
	}
	std::cout << "Admin not found" << std::endl;
}

void client_1::set_name(std::string name){
	this->_name = name;
}

void client_1::set_nick(std::string nick){
	this->_nick = nick;
}

int client_1::get_socket(){
	return this->_socket;
}

std::string client_1::get_name(){
	return this->_name;
}

std::string client_1::get_nick(){
	return this->_nick;
}

void channel::remove_admin_by_name(std::string name){
	std::vector<client_1*>::iterator it;
	//check if the client is an admin
	for(it = _admins.begin(); it != _admins.end(); it++){
		if((*it)->get_name() == name){
			(*it)->set_admin(0);
			_admins.erase(std::remove(_admins.begin(), _admins.end(), *it), _admins.end());
			send((*it)->get_socket(), "You are no longer an admin\n", strlen("You are no longer an admin\n"), 0);
			return;
		}
	}
	std::cout << "Admin not found" << std::endl;
}

client_1::client_1(){
	this->_socket = -1;
	this->_name = "";
	this->_nick = "";
	this->real_name = "";
	this->is_admin = 0;
	this->super_admin = 0;
	// this->ptr_channel = NULL; may remove it
}

void channel::set_topic_changeable(int topic_changeable){
	if(topic_changeable == 0){
		std::cout << "Topic is not changeable for non admins " << std::endl;
	}
	else if(topic_changeable == 1){
		std::cout << "Topic is changeable" << std::endl;
	}
	else{//print helping message
		std::cout << "incorrect mode " << std::endl;
	}
	this->topic_changeable = topic_changeable;
}
void client_1::set_admin(int admin){
	this->is_admin = admin;
}
client_1::~client_1(){}

int client_1::get_admin(){
	return this->is_admin;
}
std::vector<client_1*> channel::get_admins(){
	return this->_admins;
}
channel::channel(std::string name){
	this->_name = name;
	_topic = "no topic for this channel\n";
	_password = "";
	channel_mode = 1;//public 
	si_password_required = 0;
	is_max_clients_required = 1;//yes
	number_of_clients = 10;
	topic_changeable = 0;
	_password = "";
}

channel::~channel(){}

std::string channel::get_name(){
	return this->_name;
}

void channel::add_client(client_1 *cl){
	this->_clients.push_back(cl);
}
//– List all existing channels on the local server only. Your server should ignore
// parameters and list all channels and the number of users on the local server in each
// channel.
void list_command(clien_info *cl)
{
	std::string message = "List of channels:\n";
	for (size_t i = 0; i < all_channels.size(); i++)
	{
		message += all_channels[i]->get_name() + " ";
		message += ft_itos(all_channels[i]->_clients.size()) + " users\n";
		message += "Topic: " + all_channels[i]->get_topic() + "\n";
		message += "Channel mode: " + ft_itos(all_channels[i]->get_channel_mode()) + "\n";
		message += "Password required: " + ft_itos(all_channels[i]->get_si_password_required()) + "\n";
	}

	send(cl->socket, message.c_str(), message.size(), 0);
}

// void collect_data_from_user(clien_info *cl)
// {
// 	client_1 *user = new client_1();
// 	send(cl->socket,NAME_MSG, strlen(NAME_MSG), 0);
// 	keep_recv(cl, cl->name);
// 	memset(cl->nick, 0, sizeof(cl->nick));
// 	send(cl->socket, NICK_NAME_MSG, strlen(NICK_NAME_MSG), 0);
// 	keep_recv(cl, cl->nick);

// 	// print the user info just for testing
// 	std::cout << "\n==>[0]New user connected to the server:" << std::endl;
// 	std::cout << "[1]New User: " << cl->name << "[2]Nickname: " << cl->nick;
// 	std::cout << "[3]His id: " << cl->socket << std::endl;

	
// 	user->set_socket(cl->socket);
// 	user->set_name(cl->name);
// 	user->set_nick(cl->nick);
// 	user->set_admin(0);
	
// 	// add the new client to the list of clients
// 	all_clients.push_back(user);
// 	std::cout << "New user added to the list of clients" << std::endl;
// 	std::cout << "Number of clients: " << all_clients.size() << "\n\n";
// 	while(1)
// 	{
// 	std::cout << "Number of clients: " << all_clients.size() << std::endl;
// 	std::cout << "Number of channels: " << all_channels.size() << std::endl;
// 	// bzero(cl->cmd, sizeof(cl->cmd));
// 	// cl->tokens.clear();
// 	send(cl->socket, "Enter a command: ", strlen("Enter a command: "), 0);
// 	keep_recv(cl, cl->cmd);
// 	cl->tokens = split(cl->cmd, ' ');
// 	if(cl->tokens.size() == 0){
// 		exit(1);
// 	}
	
// 	if(cl->tokens[0] == "/leave\n")
// 	{
// 	    std::cout << "User quit" << std::endl;
// 	    break;
// 	}

// 	if(cl->tokens[0] == "/join")
// 	{
// 		join_command(cl, user);
// 	}

// 	else if(cl->tokens[0] == "/list\n")
// 	{
// 		std::cout << "List command" << std::endl;
// 		list_command(cl);
// 	}

// 	}
// }

void channel::set_is_max_clients_required(int is_max_clients_required){
	if(is_max_clients_required == 0){
		std::cout << "and set to " << this->number_of_clients << std::endl;
	}
	else if(is_max_clients_required == 1){
		std::cout << "Max clients is not required" << std::endl;
	}
	else{//print helping message
		std::cout << "incorrect mode " << std::endl;
	}
	this->is_max_clients_required = is_max_clients_required;
}

void channel::set_number_of_clients(size_t number_of_clients){
	std::cout << "Number of clients set to: " << number_of_clients << std::endl;
	if(number_of_clients <= 0){
		std::cout << "Number of clients must be greater than 0" << std::endl;
	}
	else if (number_of_clients > 100){
		std::cout << "Number of clients must be less than 10" << std::endl;
	}
	else if(number_of_clients < this->_clients.size()){
		std::cout << "Number of clients must be less than the number of clients in the channel" << std::endl;
	}
	else
		this->number_of_clients = number_of_clients;
}

size_t channel::get_number_of_clients(){
	return this->number_of_clients;
}

int channel::get_is_max_clients_required(){
	return this->is_max_clients_required;
}