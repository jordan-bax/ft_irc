
#include "../classes/client.hpp"
#include "../Messages.hpp"
#include "../classes/client_exception.hpp"
#include <sstream>

void	client::help(std::vector<std::string> input)
{
	send_message(_nick_name, messages::client_message::HELP_MESSAGE);
}

// void	client::user(std::vector<std::string> input)
// {
// 	if (input.size() > 2)
// 		throw(client_exception(messages::client_message::ERROR_TOO_MANY_PAR));
// 	if (input.size() < 2)
// 		throw(client_exception(messages::client_message::ERROR_TOO_FEW_PAR));
// 	_full_name = input[1];
// 	buf_write = "Username has been set to: " + _full_name + "\n";
// }

void	client::nick(std::vector<std::string> input) {
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NONICKNAMEGIVEN));
	
	// TODO: throw exception when nick in use, erroneous or blocked

	_nick_name = input[1];
}

void	client::pass(std::vector<std::string> input) {
	if (!_password.empty())
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, input[0]));
	if (!_full_name.empty() || !_nick_name.empty())
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	_password = input[1];
}
