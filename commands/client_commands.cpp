
#include "../classes/client.hpp"
#include "../classes/channel.hpp"
#include "../Messages.hpp"
#include "../classes/client_exception.hpp"
#include <sstream>
#include <algorithm>
#include <map>
#include <limits>
#include <unordered_map>
#include <functional>

// TODO: check if client allows custom numeric codes

void	client::help(std::vector<std::string> input, env &server_env) {
	help_message(server_env);
}

void	client::send_usrmsg(std::string const &target, std::string const &msg, env &server_env) {
	client *target_client;

	if (!(target_client = server_env.search_client_nick( target)))
		throw(client_exception(messages::Client::ERR_NOSUCHNICK, {target}));
	target_client->receive_message(server_env, *_user, msg, "PRIVMSG");
}

void	client::send_chanmsg(std::string const &target, std::string const &msg, env &server_env) {
	channel	*target_channel;

	if (!(target_channel = server_env.search_channel( target)))
		throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {target}));
	if (!target_channel->user_in_channel(get_nick()))
		throw(client_exception(messages::Client::ERR_NOTONCHANNEL, {target}));
	target_channel->send_message(server_env, *_user, get_nick(), msg);
}

void	client::privmsg(std::vector<std::string> input, env &server_env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 2 )
		throw(client_exception(messages::Client::ERR_NORECIPIENT, {input[0]}));
	if (input.size() < 3 && input[1][0] == ':')
		throw(client_exception(messages::Client::ERR_NORECIPIENT, {input[0]}));
	else if (input.size() < 3)
		throw(client_exception(messages::Client::ERR_NOTEXTTOSEND));

	std::string target = input[1];
	std::string msg = input[2];
	if (msg[0] == ':')
		msg.erase(msg.begin(), msg.begin() + 1);
	if (std::string("#&+!").find(input[1][0]) != std::string::npos)
		send_chanmsg(target, msg, server_env);
	else
		send_usrmsg(target, msg, server_env);
}

void client::user(std::vector<std::string> input, env &server_env) {
	if (input.size() < 5)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	if (_user != NULL)
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (_tmp_nick.empty())
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input[4][0] != ':')
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS));
	if (!server_env.nick_available( _tmp_nick))
		throw(client_exception(messages::Client::ERR_NICKNAMEINUSE, {_tmp_nick}));

	std::string	full_name = input[4].substr(1);
	_user = new User_data(_fd);
	_user->set_nickname(_tmp_nick);
	_user->set_fullname(full_name);
	_user->set_username(input[1]);
	login_messages(server_env);
}

void	client::nick(std::vector<std::string> input, env &server_env) {
	if (_user != NULL)
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NONICKNAMEGIVEN));
	
	// TODO: throw exception when nick in use, erroneous or blocked
	if (!server_env.nick_available( input[1]))
		throw(client_exception(messages::Client::ERR_NICKNAMEINUSE, {input[1]}));
	_tmp_nick = input[1];
}

void	client::pass(std::vector<std::string> input, env &server_env) {
	if (_authorised)
		throw(client_exception(messages::Client::ERR_ALREADYREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	if (input[1] != server_env.get_pass())
		throw(client_exception(messages::Client::ERR_PASSWDMISMATCH));
	_authorised = true;
}

void	client::join(std::vector<std::string> input, env &server_env) {
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
		if (server_env.channel_exists( channels[i])) {
			_channels.push_back(server_env.join_channel( channels[i], this, key));
			_channels.back()->send_mode_message(server_env, *_user, "JOIN");
		}
		else
			_channels.push_back(server_env.new_channel( channels[i], this, key));
	}
}

// TODO: kick messages when user is kicked from channel
void	client::kick(std::vector<std::string> input, env &server_env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 3)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	
	std::vector<std::string> channels = split(input[1], ',');
	std::vector<std::string> users = split(input[2], ',');
	if (channels.size() > 1 && channels.size() != users.size())
		throw(client_exception(messages::Client::ERR_CHANNOTUSERSIZE, {input[0]}));

	for (size_t i = 0; i < users.size(); i++) {
		std::string chan_name = channels.size() > 1 ? channels[i] : channels[0];
		std::string user_name = users[i];

		channel *channel = server_env.search_channel( chan_name);
		if (!channel)
			throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {chan_name}));
		if (!channel->user_in_channel(get_nick()))
			throw(client_exception(messages::Client::ERR_NOTONCHANNEL, {chan_name}));
		if (user_name != get_nick() && !channel->user_is_operator(get_nick()))
			throw(client_exception(messages::Client::ERR_CHANOPRIVSNEEDED, {chan_name}));
		if (!channel->user_in_channel(user_name))
			throw(client_exception(messages::Client::ERR_USERNOTINCHANNEL, {user_name, chan_name}));
		channel->remove_client(user_name);
	}
}

void	client::topic(std::vector<std::string> input, env &server_env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 2)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	if (input.size() > 2 && input[2][0] != ':')
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));

	channel *channel = server_env.search_channel( input[1]);
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

void	client::invite(std::vector<std::string> input, env &server_env) {
	if (_user == NULL)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 3)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	
	client	*client = server_env.search_client_nick( input[1]);
	channel *channel = server_env.search_channel( input[2]);
	if (!client)
		throw(client_exception(messages::Client::ERR_NOSUCHNICK, {input[1]}));
	if (!channel) {
		client->send_numeric_reply(server_env, messages::Client::RPL_INVITING, "", {get_nick(), client->get_nick(), input[2]});
		send_numeric_reply(server_env, messages::Client::RPL_INVITING, "", {get_nick(), client->get_nick(), input[2]});
		return ;
	}
	
	if (!channel->user_in_channel(get_nick()))
		throw(client_exception(messages::Client::ERR_NOTONCHANNEL, {input[2]}));
	if (channel->get_invonly() && !channel->user_is_operator(get_nick()))
		throw(client_exception(messages::Client::ERR_CHANOPRIVSNEEDED, {channel->get_name()}));
	if (channel->user_in_channel(client->get_nick()))
		throw(client_exception(messages::Client::ERR_USERONCHANNEL, {client->get_nick(), channel->get_name()}));
	channel->add_invite(input[1]);
	send_numeric_reply(server_env, messages::Client::RPL_INVITING, "", {get_nick(), client->get_nick(), channel->get_name()});
	client->receive_message(server_env, *_user, channel->get_name(), "INVITE");
}

void	client::handle_i(env const &server_env, std::vector<std::string> input, channel *chan) {
	if (input[2][0] == '-') {
		chan->set_invonly(false);
		chan->send_mode_message(server_env, *_user, "MODE", "-i");
	}
	else {
		chan->set_invonly(true);
		chan->send_mode_message(server_env, *_user, "MODE", "+i");
	}
}

void	client::handle_t(env const &server_env, std::vector<std::string> input, channel *chan) {
	if (input[2][0] == '-') {
		chan->set_topic_permission(false);
		chan->send_mode_message(server_env, *_user, "MODE", "-t");
	}
	else {
		chan->set_topic_permission(true);
		chan->send_mode_message(server_env, *_user, "MODE", "+t");
	}
}

void	client::handle_k(env const &server_env, std::vector<std::string> input, channel *chan) {
	if (input[2][0] == '-' && chan->check_key(""))
		return ;
	if (input.size() < 4)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[2]}));
	if (input[2][0] == '-') {
		if (!chan->check_key(input[3]))
			throw(client_exception(messages::Client::ERR_BADCHANNELKEY, {chan->get_name()}));
		chan->set_key("");
		chan->send_mode_message(server_env, *_user, "MODE", "-k");
		return ;
	}
	chan->set_key(input[3]);
	chan->send_mode_message(server_env, *_user, "MODE", "+k " + input[3]);
}

void	client::handle_o(env const &server_env, std::vector<std::string> input, channel *chan) {
	if (input.size() < 4)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[2]}));
	if (!chan->user_in_channel(input[3]))
		throw(client_exception(messages::Client::ERR_USERNOTINCHANNEL, {input[3], chan->get_name()}));

	if (input[2][0] == '-') {
		if (!chan->user_is_operator(input[3]))
			return ;
		chan->remove_operator(input[3]);
		chan->send_mode_message(server_env, *_user, "MODE", "-o " + input[3]);
		return ;
	}
	chan->add_operator(input[3]);
	chan->send_mode_message(server_env, *_user, "MODE", "+o " + input[3]);
}

void	client::handle_l(env const &server_env, std::vector<std::string> input, channel *chan) {
	if (input[2][0] == '-') {
		chan->set_limit(0);
		chan->send_mode_message(server_env, *_user, "MODE", "-l");
		return ;
	}
	if (input.size() != 4)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[2]}));

	unsigned long new_value = 0;
    for (char c : input[3]) {
		if (!std::isdigit(c))
			throw(client_exception(messages::Client::ERR_INVALIDLIMIT, {input[2]}));
        new_value = new_value * 10 + (c - '0');
    }
	if (new_value > std::numeric_limits<unsigned int>::max()) {
        throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[2]}));
    }
	chan->set_limit(static_cast<unsigned int>(new_value));
	chan->send_mode_message(server_env, *_user, "MODE", "+l " + input[3]);
}

const std::unordered_map<char, client::ModePtr> client::mode_handlers = {
        {'i', &client::handle_i},
        {'t', &client::handle_t},
        {'k', &client::handle_k},
        {'o', &client::handle_o},
        {'l', &client::handle_l}
    };

// TODO: add messages to confirm changes to modes
void	client::mode(std::vector<std::string> input, env &server_env) {
	if (!_user)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (input.size() < 3)
		throw(client_exception(messages::Client::ERR_NEEDMOREPARAMS, {input[0]}));
	
	channel	*target_channel = server_env.search_channel( input[1]);
	if (!target_channel)
		throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {input[1]}));
	if (!target_channel->user_is_operator(get_nick()))
		throw(client_exception(messages::Client::ERR_CHANOPRIVSNEEDED, {target_channel->get_name()}));
	if (input[2].length() != 2)
		throw(client_exception(messages::Client::ERR_UNKNOWNMODE, {input[2], target_channel->get_name()}));
	if (input[2][0] != '-' && input[2][0] != '+')
		throw(client_exception(messages::Client::ERR_UMODEUNKNOWNFLAG));

	char	mode_char = input[2][1];
	auto	it = mode_handlers.find(mode_char);
	if (it == mode_handlers.end())
		throw(client_exception(messages::Client::ERR_UNKNOWNMODE, {input[2], target_channel->get_name()}));
	(this->*(it->second))(server_env, input, target_channel);
}

void	client::list(std::vector<std::string> input, env &server_env) {
	if (!_user)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
		
	if (input.size() < 2) {
		std::vector<channel*> channels = server_env.get_channels();
		for (channel *chan : channels) {
			std::string	msg = chan->get_name() + " " + std::to_string(chan->get_user_count()) + " :" + chan->get_topic();
			send_numeric_reply(server_env, messages::Client::RPL_LIST, msg);
		}
		send_numeric_reply(server_env, client_exception(messages::Client::RPL_LISTEND));
		return ;
	}

	std::vector<std::string> channels = split(input[1], ',');
	for (std::string chan_name : channels) {
		if (!server_env.channel_exists(chan_name)) {
			send_numeric_reply(server_env, client_exception(messages::Client::ERR_NOSUCHCHANNEL, {chan_name}));
			continue ;
		}
		channel *chan = server_env.search_channel(chan_name);
		std::string msg = chan->get_name() + " " + std::to_string(chan->get_user_count()) + " :" + chan->get_topic();
		send_numeric_reply(server_env, messages::Client::RPL_LIST, msg);
	}
	send_numeric_reply(server_env, client_exception(messages::Client::RPL_LISTEND));
}
