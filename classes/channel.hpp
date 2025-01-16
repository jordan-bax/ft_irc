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
	bool			_topic_permision;
	std::string		_topic_msg;
	unsigned int	_limit;
	std::vector<client*> _clients;
	std::vector<std::string> _operators;

	channel();

public:
	channel( std::string name );
	channel( channel const & src );
	~channel();

	channel & operator=( channel const & rhs );

	static void		clear(std::vector<channel*> &start)
	{
		while (!start.empty())
		{
			delete start.back();
			start.pop_back();
		}
		start.shrink_to_fit();
	};

	void	add_client(client *client);
	void	remove_client(std::string nick_name);
	void	add_operator(client *client);

	bool	user_in_channel(std::string name);
	bool	user_is_operator(std::string name);

	void	set_key(std::string key) { _password = key; };
	bool	check_key(std::string const key) { return (key == _password); }
	std::string	const	&get_name() const { return (_name); }
	std::string	const	&get_topic() const { return (_topic_msg); }
	void				set_topic(std::string const topic) { _topic_msg = topic; }
	void				clear_topic() { _topic_msg.clear(); }
	bool const			get_topic_permission() const { return (_topic_permision); }
	unsigned int const	is_full() const { return (_clients.size() >= _limit); }
	bool const			&get_invonly() const { return (_invite_only); }

	void	send_message(std::string const &sender, std::string const &msg);

	static bool	valid_name(std::string name);

};

std::ostream & operator<<( std::ostream & o, channel const & rhs);
