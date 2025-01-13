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
	std::vector<client*> _operators;

	channel();

public:
	channel( std::string name );
	channel( channel const & src );
	~channel();

	channel & operator=( channel const & rhs );

	void	add_client(client *client);
	void	remove_client(std::string nick_name);
	void	add_operator(client *client);

	bool	user_in_channel(std::string name);

	std::string	const &get_name() const { return (_name); }

};

std::ostream & operator<<( std::ostream & o, channel const & rhs);
