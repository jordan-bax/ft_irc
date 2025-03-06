#pragma once
#include <iostream>
#include <vector>


#define DEFAULT_LIMIT 100

class client;
class User_data;
class env;

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
	std::vector<std::string> _invites;

	channel();

public:
	channel( std::string name );
	channel( channel const & src );
	~channel();

	channel & operator=( channel const & rhs );

	static void		clear(std::vector<channel*> &start);

	void	add_client(client *client);
	void	remove_client(std::string nick_name);
	void	add_operator(std::string user_name);
	void	remove_operator(std::string user_name);
	void	add_invite(std::string user_name);
	void	remove_invite(std::string name);

	bool	user_in_channel(std::string name);
	bool	user_is_operator(std::string name);
	bool	user_is_invited(std::string name);

	bool				check_key(std::string const key);
	void				clear_topic();
	bool const			is_full() const;

	void	set_key(std::string key);
	void	set_topic(std::string const topic);
	void	set_topic_permission(bool permission);
	void	set_invonly(bool invonly);
	void	set_limit(unsigned int limit);

	std::string	const	&get_name() const ;
	std::string	const	&get_topic() const ;
	bool const			get_topic_permission() const ;
	bool const			&get_invonly() const ;
	unsigned int		get_user_count() const;

	void	send_message(env const &env, User_data const &sender, std::string const &sender_nick, std::string const &msg);
	void	send_mode_message(env const &env, User_data const &sender, std::string const &cmd, std::string const &msg = "");

	static bool	valid_name(std::string name);

};

std::ostream & operator<<( std::ostream & o, channel const & rhs);
