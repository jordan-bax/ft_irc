#pragma once
#include "connection.hpp"
#include "../Messages.hpp"
#include "client_exception.hpp"
#include <unordered_map>

#define SERVER_PASS "password"
#define MAX_CHANNELS 5

class client : public connection
{
private:
	bool					_authorised = false;
	std::string				_tmp_nick;
	User_data				*_user = NULL;
	std::vector<channel*>	_channels;

	bool	commands();
	void	handle_client_input(s_env *env);

	// command functions
	void	help(std::vector<std::string> input, s_env *env);
	void	privmsg(std::vector<std::string> input, s_env *env);
	void	join(std::vector<std::string> input, s_env *env);
	void	user(std::vector<std::string> input, s_env *env);
	void	nick(std::vector<std::string> input, s_env *env);
	void	pass(std::vector<std::string> input, s_env *env);
	void	kick(std::vector<std::string> input, s_env *env);
	void	invite(std::vector<std::string> input, s_env *env);
	void	topic(std::vector<std::string> input, s_env *env);
	void	mode(std::vector<std::string> input, s_env *env);

	void	send_usrmsg(std::string const &target, std::string const &msg, s_env *env);
	void	send_chanmsg(std::string const &target, std::string const &msg, s_env *env);
public:
	client();
	client( int type , int fd );
	// client( client const & src );
	~client();

	using FunctionPtr = void (client::*)(std::vector<std::string>, s_env*);
	static const std::unordered_map<std::string, FunctionPtr> functionMap;

	// client & operator=( client const & rhs );
	void	write(void);
	bool	read(s_env *env);

	std::string	build_reply_message(messages::Client code, std::string const &msg, std::vector<std::string> params);
	std::string	reply_message(client_exception const &e);
	void		send_numeric_reply(client_exception const &e);
	void		send_numeric_reply(messages::Client code, std::string const &msg, std::vector<std::string> params);
	void		receive_message(std::string const &sender, std::string const &msg);
	void		client_message(std::string const &msg);

	std::string	const &get_nick() const {return (_user->get_nickname());}
	bool	is_registered() const { return (_user != NULL); }

	static std::vector<std::string>	split(std::string const &str, char delimiter);
};

// std::ostream & operator<<( std::ostream & o, client const & rhs);
