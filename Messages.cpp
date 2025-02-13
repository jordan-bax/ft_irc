
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
		{messages::Client::RPL_AWAY, " :User is currently offline"},
		{messages::Client::RPL_LISTEND, ":End of LIST"},
		{messages::Client::RPL_NOTOPIC, " :No topic is set"},
		{messages::Client::RPL_TOPIC, " :"},
		{messages::Client::RPL_INVITING, " :"},
		{messages::Client::ERR_NOSUCHNICK, " :No such nick/channel"},
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
		{messages::Client::ERR_ALREADYREGISTERED, ":Unauthorized command (already registered)"},
		{messages::Client::ERR_PASSWDMISMATCH, ":Password incorrect"},
		{messages::Client::ERR_NOSUCHCHANNEL, " :No such channel"},
		{messages::Client::ERR_INVITEONLYCHAN, " :Cannot join channel (+i)"},
		{messages::Client::ERR_CHANNELISFULL, " :Cannot join channel (+l)"},
		{messages::Client::ERR_BADCHANNELKEY, " :Cannot join channel (+k)"},
		{messages::Client::ERR_TOOMANYCHANNELS, " :You have joined too many channels"},
		{messages::Client::ERR_USERNOTINCHANNEL, " :They aren't on that channel"},
		{messages::Client::ERR_NOTONCHANNEL, " :You're not on that channel"},
		{messages::Client::ERR_CHANOPRIVSNEEDED, " :You're not channel operator"},
		{messages::Client::ERR_USERONCHANNEL, " :User is already on channel"},
		{messages::Client::ERR_UNKNOWNMODE, " :is unknown mode to me for "},
		{messages::Client::ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag"}
	};

	auto it = reply_map.find(numeric_reply);
	if (it != reply_map.end())
		return (it->second);
	throw (server_exception("Unknown numeric reply"));
}
