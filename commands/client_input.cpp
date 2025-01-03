
#include "../classes/client.hpp"
#include "../classes/client_exception.hpp"
#include "../Messages.hpp"
#include <iostream>
#include <sstream>
#include <iterator>

std::vector<std::string>	split(std::string const &str, char delimiter) {
	std::vector<std::string>	result;
	std::string					token;
	std::istringstream			token_stream(str);

	while (std::getline(token_stream, token, delimiter)) {
		size_t	pos;
		while ((pos = token.find('\n')) != std::string::npos)
			token.erase(pos, 1);
		if (token.length() == 0)
			continue ;
		result.push_back(token);
	}
	return (result);
}

const std::unordered_map<std::string, client::FunctionPtr> client::functionMap = {
    {"HELP", &client::help},
    {"PRIVMSG", NULL},
    {"JOIN", NULL},
	{"USER", &client::user},
	{"NICK", &client::nick},
	{"PASS", &client::pass},
	{"KICK", NULL},
	{"INVITE", NULL},
	{"TOPIC", NULL},
	{"MODE", NULL}
};

static std::unordered_map<std::string, client::FunctionPtr>::const_iterator verify_input(std::vector<std::string> const &input)
{
	std::unordered_map<std::string, client::FunctionPtr>::const_iterator i = client::functionMap.find(input.front());
	if (i == client::functionMap.end())
		throw(client_exception("Invalid command: " + input.front() + '\n'));
	return (i);
}

void	client::handle_client_input()
{
	std::unordered_map<std::string, FunctionPtr>::const_iterator i;
	std::vector<std::string> input = split(buf_read, ' ');

	if (input.empty())
		throw(client_exception(messages::client::INPUT_USAGE));
	i = verify_input(input);
	(this->*(i->second))(input);
}
