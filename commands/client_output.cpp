
#include "../classes/client.hpp"
#include "../classes/server_exception.hpp"
#include "../Messages.hpp"
#include <sstream>

// TODO: check if write() doesn't create problems elsewhere

std::string	client::reply_message(messages::Client numeric_reply, std::string const &param = "") {
	std::string	msg = get_client_message(numeric_reply);

	switch (numeric_reply) {
		case messages::Client::ERR_UNKNOWNCOMMAND:
		case messages::Client::ERR_ERRONEUSNICKNAME:
		case messages::Client::ERR_NICKNAMEINUSE:
		case messages::Client::ERR_UNAVAILRESOURCE:
		case messages::Client::ERR_NEEDMOREPARAMS:
			if (param.empty())
				throw(server_exception("Error: missing paramater"));
			return (param + msg);
		default:
			return (msg);
	}
}

void	client::send_numeric_reply(int numeric_reply, std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << "<servername>" << " " << numeric_reply << " " << _nick_name << " :" << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}

void	client::send_message(std::string const &target, std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << _nick_name << " PRIVMSG " << target << " :" << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}
