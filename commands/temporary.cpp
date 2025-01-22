
#include "temporary.hpp"
#include "../classes/client_exception.hpp"
#include "../Messages.hpp"

std::vector<client*> get_clients(s_env *env) {
	std::vector<client*> clients;

	for (auto &connection : env->connections) {
		if (connection->get_type() == FD_CLIENT) {
			if (client* client_ptr = dynamic_cast<client*>(connection)) {
				clients.push_back(client_ptr);
			}
		}
	}
	return (clients);
}

client	*search_client_nick(s_env *env, std::string nick_name)
{
	std::vector<client*> clients = get_clients(env);

	for (auto *client: clients) {
		if (client->is_registered() && client->get_nick() == nick_name)
			return (client);
	}
	return (NULL);
}

bool	nick_available(s_env *env, std::string nick_name)
{
	std::vector<client*> clients = get_clients(env);

	for (auto *client: clients) {
		if (client->is_registered() && client->get_nick() == nick_name)
			return (false);
	}
	return (true);
}

bool	channel_exists(s_env *env, std::string name) {
	return (search_channel(env, name) != NULL);
}

channel *search_channel(s_env *env, std::string name) {
	for (auto *channel: env->channels) {
		if (channel->get_name() == name)
			return (channel);
	}
	return (NULL);
}

channel	*new_channel(s_env *env, std::string name, client *creator, std::string key) {
	if (channel_exists(env, name))
		throw(client_exception(messages::Client::ERR_UNAVAILRESOURCE, {name}));
	channel *new_channel = new channel(name);
	new_channel->add_client(creator);
	new_channel->add_operator(creator->get_nick());
	new_channel->set_key(key);
	env->channels.push_back(new_channel);
	return (new_channel);
}

// TODO: alot (check for key for example)
// change behavior when user is already in channel
channel *join_channel(s_env *env, std::string name, client *client, std::string key) {
	if (!channel_exists(env, name))
		throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {name}));

	for (auto *channel: env->channels) {
		if (channel->get_name() == name) {
			if (!channel->check_key(key) && !channel->user_is_invited(client->get_nick()))
				throw(client_exception(messages::Client::ERR_BADCHANNELKEY, {name}));
			if (channel->user_in_channel(client->get_nick()))
				throw(client_exception(messages::Client::ERR_UNAVAILRESOURCE, {channel->get_name()}));
			if (channel->is_full())
				throw(client_exception(messages::Client::ERR_CHANNELISFULL, {channel->get_name()}));
			if (channel->get_invonly() && !channel->user_is_invited(client->get_nick()))
				throw(client_exception(messages::Client::ERR_INVITEONLYCHAN, {channel->get_name()}));
			channel->add_client(client);
			if (channel->user_is_invited(client->get_nick()))
				channel->remove_invite(client->get_nick());
			return (channel);
		}
	}
	return (NULL);
}
