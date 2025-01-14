
#include "channel.hpp"
#include "client.hpp"
#include <algorithm>

channel::channel() {}

channel::channel( std::string name ): _name(name), _invite_only(false), _limit(DEFAULT_LIMIT) {
	std::cout << "new channel [" << _name << "]\n";
}

channel::channel( channel const & src ) {
	*this = src;
}

channel::~channel() {
	std::cout << "delete channel [" << _name << "]\n";
}

channel & channel::operator=( channel const & rhs ) {
	_name = rhs._name;
	_password = rhs._password;
	_invite_only = rhs._invite_only;
	_topic = rhs._topic;
	_limit = rhs._limit;
	_clients = rhs._clients;
	_operators = rhs._operators;
	return (*this);
}

void	channel::add_client(client *client) {
	if (client != NULL)
		_clients.push_back(client);
}

void	channel::remove_client(std::string nick_name) {
	for (int i = 0; i < _clients.size(); i++) {
		if (_clients[i]->get_nick() == nick_name)
			_clients.erase(_clients.begin() + i);
	}
}

void	channel::add_operator(client *client) {
	if (client != NULL)
		_operators.push_back(client->get_nick());
}

bool	channel::user_in_channel(std::string name) {
	for (client *client: _clients) {
		if (client->get_nick() == name)
			return (true);
	}
	return (false);
}

void	channel::send_message(std::string const &sender, std::string const &msg) {
	for (auto *client: _clients) {
		if (client->get_nick() != sender)
			client->receive_message(_name, msg);
	}
}

bool	channel::valid_name(std::string name) {
	if (name.empty() || name.length() > 200)
		return (false);
	if (std::string("#&+!").find(name[0]) == std::string::npos)
		return (false);
	return std::all_of(name.begin() + 1, name.end(), [](char c) {
		return std::isalnum(c) || c == '-' || c == '_' || c == '.';
	});
}
