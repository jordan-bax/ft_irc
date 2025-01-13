
#ifndef MESSAGES_HPP
# define MESSAGES_HPP

#include <string>

namespace messages {
	namespace client_message {
		constexpr const char *HELP_MESSAGE = 
    		"\nWelcome to the IRC server! Available commands are:\n"
    		"HELP          : Displays this help message.\n"
    		"PRIVMSG <msgtarget> :<message> : Sends a private message to a user.\n"
    		"JOIN <channel> : Joins the specified channel.\n"
    		"USER <user> <unused> <unused> <realname> : Sets the username for the client.\n"
    		"NICK <nickname> : Sets the nickname for the client.\n"
    		"PASS <password> : Provides the password for authentication.\n"
    		"KICK <channel> <user> : Kicks a user from a channel.\n"
    		"INVITE <user> <channel> : Invites a user to a channel.\n"
    		"TOPIC <channel> [<topic>] : Views or changes the topic of a channel.\n"
    		"MODE <channel> <mode> : Changes the mode of a channel or user.\n"
    		"For more detailed information, please refer to the server documentation.\n\n";

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
		ERR_NOTEXTTOSEND = 412			// returned when user hasn't entered text to send
		
	};

	namespace server {

	}
}

std::string	get_client_message(messages::Client numeric_reply);

#endif
