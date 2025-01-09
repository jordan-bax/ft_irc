
#include "../classes/client.hpp"
#include "../Messages.hpp"
#include "../classes/client_exception.hpp"
#include "temporary.hpp"
#include <sstream>

void	client::help(std::vector<std::string> input, s_env *env)
{
	send_message(_nick_name, messages::client_message::HELP_MESSAGE);
}

// static std::string	extract_message(const std::vector<std::string> &input) {
// 	std::string	msg;
// 	bool first = true;

// 	for (std::string const &word: input) {
// 		if (first) {
// 			msg += word.substr(1);
// 			first = false;
// 		} else
// 			msg += ' ' + word;
// 	}
// 	return (msg);
// }

void	client::privmsg(std::vector<std::string> input, s_env *env)
{
	if (_user_name.empty())
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 2 )
		throw(client_exception(messages::Client::ERR_NORECIPIENT, input[0]));
	int	i = 1;
	while (i < input.size()) {
		if (input[i][0] == ':')
			break ;
		i++;
	}
	if (i == input.size())
		throw(client_exception(messages::Client::ERR_NOTEXTTOSEND));

	std::string	target;
	for (int j = 1; j < i; j++)
		target += input[j];
	if (target.length() < 1)
		throw(client_exception(messages::Client::ERR_NORECIPIENT, input[0]));
	client *target_client;
	if (!(target_client = search_client_nick(env, target)))
		throw(client_exception(messages::Client::ERR_NOSUCHNICK, target));

	std::string	msg;
	for (int j = i; j < input.size(); j++) {
		if (j == i)
			input[j].erase(0, 1);
		else
			msg += ' ';
		msg += input[j];
	}
	target_client->receive_message(_nick_name, msg);
}

// TODO possibly add custom numeric reply codes if available clients allow
void client::user(std::vector<std::string> input, s_env *env) {
	if (input.size() < 5)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, input[0]));
	if (!_user_name.empty())
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (_nick_name.empty())
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input[4][0] != ':')
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS));
	
	std::string	full_name;
	for (int i = 4; i < input.size(); i++) {
		if (i == 4)
			input[i].erase(0, 1);
		else
			full_name += ' ';
		full_name += input[i];
	}
	_full_name = full_name;
	_user_name = input[1];
}

void	client::nick(std::vector<std::string> input, s_env *env) {
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NONICKNAMEGIVEN));
	
	// TODO: throw exception when nick in use, erroneous or blocked
	if (!nick_available(env, input[1]))
		throw(client_exception(messages::Client::ERR_NICKNAMEINUSE, input[1]));
	_nick_name = input[1];
}

void	client::pass(std::vector<std::string> input, s_env *env) {
	if (!_password.empty())
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, input[0]));
	if (!_full_name.empty() || !_nick_name.empty())
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	_password = input[1];
}
