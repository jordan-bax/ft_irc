
#include "../classes/client.hpp"
#include "../classes/server_exception.hpp"
#include "../Messages.hpp"
#include <sstream>

// TODO: check if write() doesn't create problems elsewhere

std::string	client::reply_message(messages::Client numeric_reply, std::string const &param = "") {
	std::string	msg = get_client_message(numeric_reply);

	switch (numeric_reply) {
		case messages::Client::ERR_NOSUCHNICK:
		case messages::Client::ERR_CANNOTSENDTOCHAN:
		case messages::Client::ERR_UNKNOWNCOMMAND:
		case messages::Client::ERR_ERRONEUSNICKNAME:
		case messages::Client::ERR_NICKNAMEINUSE:
		case messages::Client::ERR_UNAVAILRESOURCE:
		case messages::Client::ERR_NEEDMOREPARAMS:
			if (param.empty())
				throw(server_exception("Error: missing parameter"));
			return (param + msg);

		case messages::Client::ERR_NORECIPIENT:
			if (param.empty())
				throw(server_exception("Error: missing parameter"));
			return (msg + param);

		default:
			return (msg);
	}
}

void	client::send_numeric_reply(int numeric_reply, std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;
	std::string			nick_name = _nick_name.empty() ? "<not registered>" : _nick_name;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << "<servername>" << " " << numeric_reply << " " << nick_name << " :" << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}

void	client::receive_message(std::string const &sender, std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << sender << " PRIVMSG " << _nick_name << " :" << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}

void	client::send_message(std::string const &target, std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;
	std::string			nick_name = _nick_name == target ? "server" : _nick_name;
	std::string			target_name = _nick_name == target ? "you" : _nick_name;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << nick_name << " PRIVMSG " << target_name << " :" << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}
