
#include "temporary.hpp"

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
		if (client->get_nick() == nick_name)
			return (client);
	}
	return (NULL);
}

bool	nick_available(s_env *env, std::string nick_name)
{
	std::vector<client*> clients = get_clients(env);

	for (auto *client: clients) {
		if (client->get_nick() == nick_name)
			return (false);
	}
	return (true);
}
