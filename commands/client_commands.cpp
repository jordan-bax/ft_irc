
#include "../classes/client.hpp"
#include "../Messages.hpp"
#include "../classes/client_exception.hpp"
#include "temporary.hpp"
#include <sstream>
#include <map>

// TODO: check if client allows custom numeric codes

void	client::help(std::vector<std::string> input, s_env *env)
{
	client_message(messages::client_message::HELP_MESSAGE);
}

void	client::privmsg(std::vector<std::string> input, s_env *env)
{
	if (_user == NULL)
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
	target_client->receive_message(*_user, msg);
}

void client::user(std::vector<std::string> input, s_env *env) {
	if (input.size() < 5)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, input[0]));
	if (_user != NULL)
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (_tmp_nick.empty())
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
	_user = new User_data(_fd);
	_user->set_nickname(_tmp_nick);
	_user->set_fullname(full_name);
	_user->set_username(input[1]);
}

void	client::nick(std::vector<std::string> input, s_env *env) {
	if (_user != NULL)
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NONICKNAMEGIVEN));
	
	// TODO: throw exception when nick in use, erroneous or blocked
	if (!nick_available(env, input[1]))
		throw(client_exception(messages::Client::ERR_NICKNAMEINUSE, input[1]));
	_tmp_nick = input[1];
}

void	client::pass(std::vector<std::string> input, s_env *env) {
	if (_authorised)
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, input[0]));
	if (input[1] != SERVER_PASS)
		throw(client_exception(messages::Client::ERR_PASSWDMISMATCH));
	_authorised = true;
}


// TODO: implement channel key
void	client::join(std::vector<std::string> input, s_env *env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 3)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS), input[0]);
	std::vector<std::string> channels = split(input[1], ',');
	std::vector<std::string> keys = split(input[2], ',');

	for (int i = 0; i < channels.size(); i++) {
		std::string key = i < keys.size() ? keys[i] : "";
		if (channel_exists(env, channels[i]))
			_channels.push_back(join_channel(env, channels[i], key, this));
		else
			_channels.push_back(new_channel(env, channels[i], this));
	}
}
