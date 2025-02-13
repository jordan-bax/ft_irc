
#include "../classes/client.hpp"
#include "../classes/server_exception.hpp"
#include "../Messages.hpp"
#include <sstream>

std::string	client::build_reply_message(messages::Client code, std::string const &msg, std::vector<std::string> params) {
	if (msg.empty() && code != messages::Client::RPL_INVITING)
		throw(server_exception("Error: emtpy message on exception"));
	
	switch (code) {
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
			if (params.size() < 1)
				throw(server_exception("Error: missing parameter"));
			return (params[0] + msg);

		case messages::Client::ERR_NORECIPIENT:
			if (params.size() < 1)
				throw(server_exception("Error: missing parameter"));
			return (msg + params[0]);

		case messages::Client::ERR_USERNOTINCHANNEL:
		case messages::Client::ERR_USERONCHANNEL:
			if (params.size() < 2)
				throw(server_exception("Error: missing 1 or more parameters"));
			return (params[0] + " " + params[1] + msg);

		case messages::Client::RPL_TOPIC:
		case messages::Client::ERR_UNKNOWNMODE:
			if (params.size() < 2)
				throw(server_exception("Error: missing 1 or more parameters"));
			return (params[0] + msg + params[1]);

		case messages::Client::RPL_INVITING:
			if (params.size() < 3)
				throw(server_exception("Error: missing 1 or more parameters"));
			return (params[0] + " " + params[1] + " " + params[2]);

		default:
			return (msg);
	}
}

// TODO: check if write() doesn't create problems elsewhere
std::string	client::reply_message(client_exception const &e) {
	messages::Client const			&numeric_reply = e.get_numeric_reply();
	std::vector<std::string> const	&params = e.get_params();
	std::string const				&msg = get_client_message(numeric_reply);

	return (build_reply_message(numeric_reply, msg, params));
}

void	client::send_numeric_reply(env &env, client_exception const &e) {
	messages::Client	numeric_reply = e.get_numeric_reply();
	std::string			msg = reply_message(e);
	std::string			nick_name = _user == NULL ? "*" : _user->get_nickname();
	std::stringstream	ss;

	ss << ":" << env.get_hostname() << " " << numeric_reply << " " << nick_name << " " << msg << "\r\n";
	buf_write = ss.str();
}

void	client::send_numeric_reply(env &env, messages::Client code, std::string const &msg, std::vector<std::string> params) {
	std::string			full_msg = build_reply_message(code, msg, params);
	std::string			nick_name = _user == NULL ? "*" : _user->get_nickname();
	std::stringstream	ss;

	ss << ":" << env.get_hostname() << " " << code << " " << nick_name << " " << full_msg << "\r\n";
	buf_write += ss.str();
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

void		client::recieve_channel_message(std::string const &sender, std::string const &channel, std::string const &msg) {
	std::stringstream	msg_stream;
	std::stringstream	line_stream;
	std::string			line;

	msg_stream << msg;
	while (std::getline(msg_stream, line))
	{
		line_stream.str("");
		line_stream << ":" << sender << " PRIVMSG " << channel << " :" << line << "\r\n";
		buf_write = line_stream.str();
		write();
	}
}

void	client::help_message(env &env) {
	std::stringstream	msg_stream;
	std::string			line;

	msg_stream << messages::client_message::HELP_MESSAGE;
	while (std::getline(msg_stream, line)) {
		line.insert(0, ":");
		send_numeric_reply(env, messages::Client::RPL_HELP, line);
	}
}

void	client::login_messages(env &env) {
	const std::string	server_name = "IRC";
	const std::string	server_version = "v1.0";

	send_numeric_reply(env, messages::Client::RPL_WELCOME, get_client_message(messages::Client::RPL_WELCOME));
	send_numeric_reply(env, messages::Client::RPL_YOURHOST, ":Your host is " + server_name + ", running version " + server_version);
	send_numeric_reply(env, messages::Client::RPL_CREATED, ":This server was created " + env.get_date());
	send_numeric_reply(env, messages::Client::RPL_MYINFO, ":" + server_name + " " + server_version + " 0" + " 0");
}
