#pragma once
#include "connection.hpp"
#include "../Messages.hpp"
#include "client_exception.hpp"
#include <unordered_map>
#include "env.hpp"
#include "../classes/User_data.hpp"

// #define SERVER_PASS "password"
#define MAX_CHANNELS 5

class client : public connection
{
private:
protected:
	bool					_authorised = false;
	std::string				_tmp_nick;
	std::string				_address;//  Internet address. 
	uint16_t				_port;//Port number
	User_data				*_user = NULL;
	std::vector<channel*>	_channels;



	bool	commands();
	void	handle_client_input(env &server_env);

	// command functions
	void	help(std::vector<std::string> input, env &server_env);
	void	privmsg(std::vector<std::string> input, env &server_env);
	void	join(std::vector<std::string> input, env &server_env);
	void	user(std::vector<std::string> input, env &server_env);
	void	nick(std::vector<std::string> input, env &server_env);
	void	pass(std::vector<std::string> input, env &server_env);
	void	kick(std::vector<std::string> input, env &server_env);
	void	invite(std::vector<std::string> input, env &server_env);
	void	topic(std::vector<std::string> input, env &server_env);
	void	mode(std::vector<std::string> input, env &server_env);
	void	list(std::vector<std::string> input, env &server_env);

	void	send_usrmsg(std::string const &target, std::string const &msg, env &server_env);
	void	send_chanmsg(std::string const &target, std::string const &msg, env &server_env);
	client();
public:
	client( int type , int fd );
	client( int type , int fd , std::string addr, uint16_t port);
	// client( client const & src );
	virtual ~client();

	using FunctionPtr = void (client::*)(std::vector<std::string>, env&);
	static const std::unordered_map<std::string, FunctionPtr> functionMap;

	// client & operator=( client const & rhs );
	virtual void	write(void);
	virtual bool	read(env &server_env);

	std::string	build_reply_message(messages::Client code, std::string const &msg, std::vector<std::string> params);
	std::string	reply_message(client_exception const &e);
	void		send_outgoing_message(env &env, messages::Client code, std::string const &msg, std::vector<std::string> params, std::string cmd);
	void		send_numeric_reply(env &env, client_exception const &e);
	void		send_numeric_reply(env &env, messages::Client code, std::string const &msg, std::vector<std::string> params = {});
	void		receive_message(std::string const &sender, std::string const &msg);
	void		recieve_channel_message(std::string const &sender, std::string const &channel, std::string const &msg);
	void		help_message(env &env);
	void		login_messages(env &env);

	std::string	const &get_nick() const {return (_user->get_nickname());}
	bool	is_registered() const { return (_user != NULL); }

	static std::vector<std::string>	split(std::string const &str, char delimiter);
};

// std::ostream & operator<<( std::ostream & o, client const & rhs);
