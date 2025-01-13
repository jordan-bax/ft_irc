
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
	for (auto channel: env->channels) {
		if (channel.get_name() == name)
			return (true);
	}
	return (false);
}

// TODO: change array to pointers and allocate each channel
channel	*new_channel(s_env *env, std::string name, client *creator) {
	if (channel_exists(env, name))
		throw(client_exception(messages::Client::ERR_UNAVAILRESOURCE), name);
	env->channels.push_back(channel(name));
	channel *channel = &env->channels.back();
	channel->add_client(creator);
	channel->add_operator(creator);
	return (channel);
}

// TODO: alot (check for key for example)
// change behavior when user is already in channel
channel *join_channel(s_env *env, std::string name, std::string key, client *client) {
	if (!channel_exists(env, name))
		throw(client_exception(messages::Client::ERR_NOSUCHNICK, name));
	for (auto &channel: env->channels) {
		if (channel.get_name() == name) {
			if (channel.user_in_channel(client->get_nick()))
				throw(client_exception(messages::Client::ERR_UNAVAILRESOURCE), channel.get_name());
			channel.add_client(client);
			return (&channel);
		}
	}
	return (NULL);
}
