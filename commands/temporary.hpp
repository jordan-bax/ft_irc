
#ifndef TEMPORARY_HPP
#define TEMPORARY_HPP

#include "../classes/client.hpp"

std::vector<client*> get_clients(s_env *env);
client	*search_client_nick(s_env *env, std::string nick_name);
bool	nick_available(s_env *env, std::string nick_name);

bool	channel_exists(s_env *env, std::string name);
channel	*new_channel(s_env *env, std::string name, client *creator);
channel *join_channel(s_env *env, std::string name, std::string key, client *client);

#endif
