
#include "Messages.hpp"
#include "classes/server_exception.hpp"
#include <map>

// TODO fill in all the <> params

std::string	get_client_message(messages::Client numeric_reply) {
	static const std::map<messages::Client, std::string> reply_map = {
		{messages::Client::RPL_WELCOME, ":Welcome to the Internet Relay Network"},
		{messages::Client::RPL_YOURHOST, ":Your host is <servername>, running version <version>"},
		{messages::Client::RPL_CREATED, ":This server was created <date>"},
		{messages::Client::RPL_MYINFO, ":<servername> <version> <available user modes> <available channel modes>"},
		{messages::Client::ERR_NOSUCHNICK, " :No suck nick/channel"},
		{messages::Client::ERR_CANNOTSENDTOCHAN, " :Cannot send to channel"},
		{messages::Client::ERR_NORECIPIENT, ":No recipient given "},
		{messages::Client::ERR_NOTEXTTOSEND, ":No text to send"},
		{messages::Client::ERR_UNKNOWNCOMMAND, " :Unknown command"},
		{messages::Client::ERR_NONICKNAMEGIVEN, ":No nickname given"},
		{messages::Client::ERR_ERRONEUSNICKNAME, " :Erroneous nickname"},
		{messages::Client::ERR_NICKNAMEINUSE, " :Nickname is already in use"},
		{messages::Client::ERR_UNAVAILRESOURCE, " :Nick/channel is temporarily unavailable"},
		{messages::Client::ERR_NOTREGISTERED, ":You have not registered"},
		{messages::Client::ERR_NEEDMOREPARAMS, " :Not enough parameters"},
		{messages::Client::ERR_ALREADYREGISTERED, ":Unauthorized command (already registered)"}
	};

	auto it = reply_map.find(numeric_reply);
	if (it != reply_map.end())
		return (it->second);
	throw (server_exception("Unknown numeric reply"));
}
