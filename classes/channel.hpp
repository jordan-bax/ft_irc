#pragma once
#include <iostream>
#include <vector>

#define DEFAULT_LIMIT 100

class client;

class channel
{
private:
	std::string		_name;
	std::string		_password;
	bool			_invite_only;
	bool			_topic;
	unsigned int	_limit;
	std::vector<client*> _clients;
	std::vector<std::string> _operators;

	channel();

public:
	channel( std::string name );
	channel( channel const & src );
	~channel();

	channel & operator=( channel const & rhs );

};

std::ostream & operator<<( std::ostream & o, channel const & rhs);
