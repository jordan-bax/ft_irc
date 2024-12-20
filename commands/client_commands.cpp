#include "../classes/client.hpp"
#include "../classes/client_exception.hpp"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <iterator>

using FunctionPtr = void (*)();

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

static const std::unordered_map<std::string, FunctionPtr> functionMap = {
    {"HELP", NULL},
    {"PRIVMSG", NULL},
    {"JOIN", NULL},
	{"USER", NULL},
	{"NICK", NULL},
	{"PASS", NULL},
	{"KICK", NULL},
	{"INVITE", NULL},
	{"TOPIC", NULL},
	{"MODE", NULL}
};


std::unordered_map<std::string, FunctionPtr>::const_iterator verify_input(std::vector<std::string> const &input)
{
	std::unordered_map<std::string, FunctionPtr>::const_iterator i = functionMap.find(input.front());
	if (i == functionMap.end())
		throw(client_exception("Invalid command: " + input.front() + '\n'));
	return (i);
}

void	client::handle_client_input()
{
	std::unordered_map<std::string, FunctionPtr>::const_iterator i;
	std::vector<std::string> input = split(buf_read, ' ');

	if (input.empty())
		throw(client_exception("Usage: <command> [<paramaters>]\n"));
	i = verify_input(input);
}
