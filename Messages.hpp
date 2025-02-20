
#ifndef MESSAGES_HPP
# define MESSAGES_HPP

#include <string>

namespace messages {
	namespace client_message {
		constexpr const char *HELP_MESSAGE = 
    		"Welcome to the IRC server! Available commands are:\n"
    		"HELP	:Displays this help message.\n"
    		"PRIVMSG <msgtarget> :<message>		:Sends a private message to a user.\n"
    		"JOIN <channel>		:Joins the specified channel.\n"
    		"USER <user> <unused> <unused> <realname>		:Sets the username for the client.\n"
    		"NICK <nickname>		:Sets the nickname for the client.\n"
    		"PASS <password>		:Provides the password for authentication.\n"
    		"KICK <channel> <user>		:Kicks a user from a channel.\n"
    		"INVITE <user> <channel>		:Invites a user to a channel.\n"
    		"TOPIC <channel> [<topic>]		:Views or changes the topic of a channel.\n"
    		"MODE <channel> <mode>		:Changes the mode of a channel or user.\n"
    		"For more detailed information, please refer to the server documentation.\n";

		constexpr const char *INPUT_USAGE = "Usage: <command> [<paramaters>]\n";
		
		constexpr const char *ERROR_AUTH_ORDER = 
			"\nError: Incorrect authentication order\n"
			"Please authenticate in this order:\n"
			"PASS <password>\n"
			"NICK <nickname>\n"
			"USER <username>\n"
			"All authentication data has been reset!\n\n";
		constexpr const char *ERROR_TOO_MANY_PAR = 
			"Error: too many parameters\n"
			"Use HELP to get help about a command!\n";
		constexpr const char *ERROR_TOO_FEW_PAR = 
			"Error: not enough parameters\n"
			"Use HELP to get help about a command!\n";
		constexpr const char *ERROR_PASS_ALREADY_SET = "Error: password has already been set\n";
	}

	enum Client {
		RPL_WELCOME = 1,
		RPL_YOURHOST = 2,
		RPL_CREATED = 3,
		RPL_MYINFO = 4,
		RPL_AWAY = 301,
		RPL_LIST = 322,
		RPL_LISTEND = 323,
		RPL_NOTOPIC = 331,
		RPL_TOPIC = 332,
		RPL_INVITING = 341,

		ERR_UNKNOWNCOMMAND = 421,		// unknown command

		ERR_NONICKNAMEGIVEN = 431,		// returned when nickname parameter is missing
		ERR_ERRONEUSNICKNAME = 432,		// returned after a invalid nickname has been entered
		ERR_NICKNAMEINUSE = 433,		// returned when nickname already exists
		ERR_UNAVAILRESOURCE = 437,		// returned when nickname or channel is unavailable
		ERR_NOTREGISTERED = 451,		// returned when user tries to do something he need to be registered for
		ERR_NEEDMOREPARAMS = 461,		// returned when not enough paramaters have been entered
		ERR_ALREADYREGISTERED = 462,	// returned when someome tries to register when already registered
		ERR_PASSWDMISMATCH = 464,		// returned when entered password is incorrect

		ERR_NORECIPIENT = 411,			// returned when no recipient entered in privmsg
		ERR_CANNOTSENDTOCHAN = 404,		// returned when user is either not in channel or banned or mode on mute
		ERR_NOSUCHNICK = 401,			// returned when nick isn't used
		ERR_NOTEXTTOSEND = 412,			// returned when user hasn't entered text to send

		ERR_NOSUCHCHANNEL = 403,		// returned when entered channel name is invalid or doesn't exist
		ERR_INVITEONLYCHAN = 473,		// returned when user tries to enter invite only channel
		ERR_CHANNELISFULL = 471,		// returned when user tries to enter a channel that is full
		ERR_BADCHANNELKEY = 475,		// returned when user enters wrong channel key
		ERR_TOOMANYCHANNELS = 405,		// returned when user has already enterd max channel count

		ERR_USERNOTINCHANNEL = 441,		// returned when requested user isn't in channel
		ERR_NOTONCHANNEL = 442,			// returned when user isn't in entered channel
		ERR_CHANOPRIVSNEEDED = 482,		// returned when user tries to do operation only allowed by operators

		ERR_USERONCHANNEL = 443,		// returned when trying to invite user already in channel

		ERR_UNKNOWNMODE = 472,			// returned when user entered unknown mode
		ERR_UMODEUNKNOWNFLAG = 501,		// returned when user entered wrong mode flag

		// Custom codes
		RPL_HELP = 705,
		ERR_CHANNOTUSERSIZE = 706,		// returned when user doesn't give the correct amount of channels and users on kick
		ERR_INVALIDLIMIT = 707			// returned when entered limit contains other characters than numeric
	};

	namespace server {

	}
}

std::string	get_client_message(messages::Client numeric_reply);

#endif
