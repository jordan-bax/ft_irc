#pragma once
#include <iostream>
#include "../classes/channel.hpp"
# include <vector>

class connection;

class env
{
private:
	std::vector<connection*> connections;
	std::vector<channel*>    channels;
	std::string	_hostname; // hostname of the computer
	std::string	_date; // date = weekday month day hour:minutes:seconds year
	std::string	_password;
	int		_port;// the port to lissen
	int		_maxfd;// set resource soft limits
	int		_max; // the biggest fd that select will search
	int 	_select;// select returned
	fd_set	_fd_read;// a select set with the fd to read in
	fd_set	_fd_write;

public:
	env();
	env( env const & src );
	~env();
//	set and getters
	bool	set_limit();
	bool	set_host();
	bool	set_port(std::string port);
	bool	set_env(std::string port, std::string password);
	bool	set_server();
	std::string	get_pass() const;
	std::string	get_hostname() const;
	int			get_port() const;
	std::string	get_date() const;
	std::vector<connection*>&	get_connections();
	std::vector<channel*>&		get_channels();

//	main loop parts
	void	init_fd();
	void	do_select();
	void	check_fd();

	std::vector<client*> get_clients();
	client	*search_client_nick( std::string nick_name);
	bool	nick_available( std::string nick_name);

	bool	channel_exists( std::string name);
	channel *search_channel( std::string name);
	channel	*new_channel( std::string name, client *creator, std::string key = "");
	channel *join_channel( std::string name, client *client, std::string key = "");

	env & operator=( env const & rhs );
};

