
#include "../classes/client.hpp"
#include "../classes/server_exception.hpp"
#include "../Messages.hpp"
#include <sstream>

// TODO: check if write() doesn't create problems elsewhere

std::string	client::reply_message(client_exception const &e) {
	messages::Client const	&numeric_reply = e.get_numeric_reply();
	std::string	const		&p1 = e.get_p1();
	std::string const		&p2 = e.get_p2();
	std::string const		&msg = get_client_message(numeric_reply);

	switch (numeric_reply) {
		case messages::Client::RPL_AWAY:
		case messages::Client::RPL_NOTOPIC:
		case messages::Client::ERR_NOSUCHNICK:
		case messages::Client::ERR_CANNOTSENDTOCHAN:
		case messages::Client::ERR_UNKNOWNCOMMAND:
		case messages::Client::ERR_ERRONEUSNICKNAME:
		case messages::Client::ERR_NICKNAMEINUSE:
		case messages::Client::ERR_UNAVAILRESOURCE:
		case messages::Client::ERR_NEEDMOREPARAMS:
		case messages::Client::ERR_NOSUCHCHANNEL:
		case messages::Client::ERR_INVITEONLYCHAN:
		case messages::Client::ERR_CHANNELISFULL:
		case messages::Client::ERR_BADCHANNELKEY:
		case messages::Client::ERR_TOOMANYCHANNELS:
		case messages::Client::ERR_CHANOPRIVSNEEDED:
		case messages::Client::ERR_NOTONCHANNEL:
			if (p1.empty())
				throw(server_exception("Error: missing parameter"));
			return (p1 + msg);

		case messages::Client::ERR_NORECIPIENT:
			if (p1.empty())
				throw(server_exception("Error: missing parameter"));
			return (msg + p1);

		case messages::Client::ERR_USERNOTINCHANNEL:
			if (p1.empty() || p2.empty())
				throw(server_exception("Error: missing 1 or more parameters"));
			return (p1 + " " + p2 + msg);

		case messages::Client::RPL_TOPIC:
			if (p1.empty() || p2.empty())
				throw(server_exception("Error: missing 1 or more parameters"));
			return (p1 + msg + p2);

		default:
			return (msg);
	}
}

void	client::send_numeric_reply(client_exception const &e) {
	messages::Client	numeric_reply = e.get_numeric_reply();
	std::string			msg = reply_message(e);
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

void	client::receive_message(std::string const &sender, std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << sender << " PRIVMSG " << get_nick() << " :" << line << "\r\n";
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
		buf_write += line_stream.str();
		write();
	}
}
