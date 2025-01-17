
#include "../classes/client.hpp"
#include "../classes/channel.hpp"
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

void	client::send_usrmsg(std::string const &target, std::string const &msg, s_env *env) {
	client *target_client;

	if (!(target_client = search_client_nick(env, target)))
		throw(client_exception(messages::Client::ERR_NOSUCHNICK, {target}));
	target_client->receive_message(get_nick(), msg);
}

void	client::send_chanmsg(std::string const &target, std::string const &msg, s_env *env) {
	channel	*target_channel;

	if (!(target_channel = search_channel(env, target)))
		throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {target}));
	if (!target_channel->user_in_channel(get_nick()))
		throw(client_exception(messages::Client::ERR_NOTONCHANNEL, {target}));
	target_channel->send_message(get_nick(), msg);
}

void	client::privmsg(std::vector<std::string> input, s_env *env)
{
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 2 )
		throw(client_exception(messages::Client::ERR_NORECIPIENT, {input[0]}));
	if (input.size() < 3 && input[1][0] == ':')
		throw(client_exception(messages::Client::ERR_NORECIPIENT, {input[0]}));
	else if (input.size() < 3)
		throw(client_exception(messages::Client::ERR_NOTEXTTOSEND));
	if (input[2][0] != ':')
		throw(client_exception(messages::Client::ERR_NOTEXTTOSEND));

	std::string target = input[1];
	std::string msg = input[2];
	msg.erase(msg.begin(), msg.begin() + 1);
	if (std::string("#&+!").find(input[1][0]) != std::string::npos)
		send_chanmsg(target, msg, env);
	else
		send_usrmsg(target, msg, env);
}

void client::user(std::vector<std::string> input, s_env *env) {
	if (input.size() < 5)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	if (_user != NULL)
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (_tmp_nick.empty())
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input[4][0] != ':')
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS));
	
	std::string	full_name = input[4].substr(1);
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
		throw(client_exception(messages::Client::ERR_NICKNAMEINUSE, {input[1]}));
	_tmp_nick = input[1];
}

void	client::pass(std::vector<std::string> input, s_env *env) {
	if (_authorised)
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	if (input[1] != SERVER_PASS)
		throw(client_exception(messages::Client::ERR_PASSWDMISMATCH));
	_authorised = true;
}

void	client::join(std::vector<std::string> input, s_env *env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
		
	std::vector<std::string> channels = split(input[1], ',');
	std::vector<std::string> keys;
	if (input.size() > 2)
		keys = split(input[2], ',');

	for (size_t i = 0; i < channels.size(); i++) {
		std::string key = i < keys.size() ? keys[i] : "";
		if (!channel::valid_name(channels[i]))
			throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {channels[i]}));
		if (channel_exists(env, channels[i]))
			_channels.push_back(join_channel(env, channels[i], this, key));
		else
			_channels.push_back(new_channel(env, channels[i], this, key));
	}
}

// TODO: check error for more channel than 1 but not the same users
// TODO: kick messages when user is kicked from channel
void	client::kick(std::vector<std::string> input, s_env *env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 3)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	
	std::vector<std::string> channels = split(input[1], ',');
	std::vector<std::string> users = split(input[2], ',');
	if (channels.size() > 1 && channels.size() != users.size())
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));

	for (size_t i = 0; i < users.size(); i++) {
		std::string chan_name = channels.size() > 1 ? channels[i] : channels[0];
		std::string user_name = users[i];

		channel *channel = search_channel(env, chan_name);
		if (!channel)
			throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {chan_name}));
		if (!channel->user_in_channel(get_nick()))
			throw(client_exception(messages::Client::ERR_NOTONCHANNEL, {chan_name}));
		if (!channel->user_is_operator(get_nick()))
			throw(client_exception(messages::Client::ERR_CHANOPRIVSNEEDED, {chan_name}));
		if (!channel->user_in_channel(user_name))
			throw(client_exception(messages::Client::ERR_USERNOTINCHANNEL, {user_name, chan_name}));
		channel->remove_client(user_name);
	}
}

void	client::topic(std::vector<std::string> input, s_env *env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	if (input.size() > 2 && input[2][0] != ':')
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));

	channel *channel = search_channel(env, input[1]);
	if (!channel)
		throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {input[1]}));
	if (!channel->user_in_channel(get_nick()))
		throw(client_exception(messages::Client::ERR_NOTONCHANNEL, {channel->get_name()}));
	if (input.size() < 3) {
		if (channel->get_topic().empty())
			throw(client_exception(messages::Client::RPL_NOTOPIC, {channel->get_name()}));
		throw(client_exception(messages::Client::RPL_TOPIC, {channel->get_name(), channel->get_topic()}));
	}
	if (!channel->user_is_operator(get_nick()) && channel->get_topic_permission())
		throw(client_exception(messages::Client::ERR_CHANOPRIVSNEEDED, {channel->get_name()}));
	
	std::string new_topic = input[2].substr(1);
	if (new_topic.length() < 1) {
		channel->clear_topic();
		return ;
	}
	channel->set_topic(new_topic);
}

void	client::invite(std::vector<std::string> input, s_env *env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 3)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	
	client	*client = search_client_nick(env, input[1]);
	channel *channel = search_channel(env, input[2]);
	if (!client)
		throw(client_exception(messages::Client::ERR_NOSUCHNICK, {input[1]}));
	if (!channel) {
		client->send_numeric_reply(messages::Client::RPL_INVITING, "", {get_nick(), client->get_nick(), input[2]});
		send_numeric_reply(messages::Client::RPL_INVITING, "", {get_nick(), client->get_nick(), input[2]});
		return ;
	}
	
	if (!channel->user_in_channel(get_nick()))
		throw(client_exception(messages::Client::ERR_NOTONCHANNEL, {input[2]}));
	if (channel->get_invonly() && !channel->user_is_operator(get_nick()))
		throw(client_exception(messages::Client::ERR_CHANOPRIVSNEEDED, {channel->get_name()}));
	if (channel->user_in_channel(client->get_nick()))
		throw(client_exception(messages::Client::ERR_USERONCHANNEL, {client->get_nick(), channel->get_name()}));
	channel->add_invite(input[1]);
	client->send_numeric_reply(messages::Client::RPL_INVITING, "", {get_nick(), client->get_nick(), channel->get_name()});
	send_numeric_reply(messages::Client::RPL_INVITING, "", {get_nick(), client->get_nick(), channel->get_name()});
}
