
#include "../classes/client.hpp"
#include "../Messages.hpp"
#include "../classes/client_exception.hpp"

void	client::help(std::vector<std::string> input)
{
	buf_write = messages::client::HELP_MESSAGE;
}

void	client::user(std::vector<std::string> input)
{
	if (input.size() > 2)
		throw(client_exception(messages::client::ERROR_TOO_MANY_PAR));
	if (input.size() < 2)
		throw(client_exception(messages::client::ERROR_TOO_FEW_PAR));
	_full_name = input[1];
	buf_write = "Username has been set to: " + _full_name + "\n";
}

void	client::nick(std::vector<std::string> input)
{
	if (input.size() > 2)
		throw(client_exception(messages::client::ERROR_TOO_MANY_PAR));
	if (input.size() < 2)
		throw(client_exception(messages::client::ERROR_TOO_FEW_PAR));
	_nick_name = input[1];
	buf_write = "Nickname has been set to: " + _nick_name + "\n";
}

void	client::pass(std::vector<std::string> input)
{
	if (!_password.empty())
		throw(client_exception(messages::client::ERROR_PASS_ALREADY_SET));
	if (input.size() > 2)
		throw(client_exception(messages::client::ERROR_TOO_MANY_PAR));
	if (input.size() < 2)
		throw(client_exception(messages::client::ERROR_TOO_FEW_PAR));
	if (!_full_name.empty() || !_nick_name.empty())
	{
		_full_name.clear();
		_nick_name.clear();
		throw(client_exception(messages::client::ERROR_AUTH_ORDER));
	}
	_password = input[1];
	buf_write = "Password has ben set\n";
}
