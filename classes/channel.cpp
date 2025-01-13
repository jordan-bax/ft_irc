
#include "channel.hpp"

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
