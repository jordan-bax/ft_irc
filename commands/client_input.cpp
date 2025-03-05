
#include "../classes/client.hpp"
#include "../classes/client_exception.hpp"
#include "../Messages.hpp"
#include <iostream>
#include <sstream>
#include <iterator>
#include "../other/error_log.hpp"

std::vector<std::string>	client::split(std::string const &str, char delimiter) {
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
    {"PRIVMSG", &client::privmsg},
    {"JOIN", &client::join},
	{"USER", &client::user},
	{"NICK", &client::nick},
	{"PASS", &client::pass},
	{"KICK", &client::kick},
	{"INVITE", &client::invite},
	{"TOPIC", &client::topic},
	{"MODE", &client::mode},
	{"LIST", &client::list}
};

static std::unordered_map<std::string, client::FunctionPtr>::const_iterator verify_input(std::vector<std::string> const &input) {
	std::unordered_map<std::string, client::FunctionPtr>::const_iterator i = client::functionMap.find(input.front());
	if (i == client::functionMap.end())
		throw(client_exception(messages::Client::ERR_UNKNOWNCOMMAND, {input.front()}));
	return (i);
}

static std::vector<std::string>	parse_input(std::string buf) {
	std::size_t pos = buf.find('\n');
	if (pos != std::string::npos)
		buf.erase(pos, 1);
	
	std::string	tmp;
	pos = buf.find(':');
	if (pos != std::string::npos) {
		tmp = buf.substr(pos);
		buf.erase(pos);
	}

	std::vector<std::string> input = client::split(buf, ' ');
	if (!tmp.empty())
		input.push_back(tmp);
	return (input);
}

void	client::handle_client_input(env &server_env) {
	std::unordered_map<std::string, FunctionPtr>::const_iterator i;
	std::vector<std::string>	input = parse_input(buf_read);

	if (input.empty())
		return ;
	i = verify_input(input);
	if (i->first != "PASS" && i->first != "HELP" && !_authorised)
		throw(client_exception(messages::Client::ERR_NOTREGISTERED));
	if (i->second != NULL)
		(this->*(i->second))(input, server_env);
}
