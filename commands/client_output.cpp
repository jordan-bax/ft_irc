
#include "../classes/client.hpp"
#include "../classes/server_exception.hpp"
#include "../Messages.hpp"
#include <sstream>

// TODO: check if write() doesn't create problems elsewhere

std::string	client::reply_message(messages::Client numeric_reply, std::string const &param = "") {
	std::string	msg = get_client_message(numeric_reply);

	switch (numeric_reply) {
		case messages::Client::RPL_AWAY:
		case messages::Client::ERR_NOSUCHNICK:
		case messages::Client::ERR_CANNOTSENDTOCHAN:
		case messages::Client::ERR_UNKNOWNCOMMAND:
		case messages::Client::ERR_ERRONEUSNICKNAME:
		case messages::Client::ERR_NICKNAMEINUSE:
		case messages::Client::ERR_UNAVAILRESOURCE:
		case messages::Client::ERR_NEEDMOREPARAMS:
		case messages::Client::ERR_INVITEONLYCHAN:
		case messages::Client::ERR_CHANNELISFULL:
		case messages::Client::ERR_BADCHANNELKEY:
		case messages::Client::ERR_TOOMANYCHANNELS:
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
	std::string			nick_name = _user == NULL ? "<not registered>" : _user->get_nickname();

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << "127.0.0.1@localhost" << " " << numeric_reply << " " << nick_name << " " << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}

void	client::receive_message(User_data const &sender, std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << sender.get_nickname() << " PRIVMSG " << _user->get_nickname() << " :" << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}

void	client::client_message(std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << "server" << " PRIVMSG " << "you" << " :" << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}
