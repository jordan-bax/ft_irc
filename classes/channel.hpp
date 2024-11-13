#pragma once
#include <iostream>
#include <vector>

class channel
{
private:
	std::string		_name;
	std::string		_password;
	bool			_invite_only;
	bool			_topic;
	unsigned int	_limit;
	std::vector<std::string> _operators;

public:
	channel();
	channel( std::string name );
	channel( channel const & src );
	~channel();

	channel & operator=( channel const & rhs );

};

std::ostream & operator<<( std::ostream & o, channel const & rhs);
