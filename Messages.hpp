
#ifndef MESSAGES_HPP
# define MESSAGES_HPP

#include <string>

namespace messages {
	namespace client {
		constexpr const char *HELP_MESSAGE = 
    		"\nWelcome to the IRC server! Available commands are:\n"
    		"HELP          : Displays this help message.\n"
    		"PRIVMSG <msgtarget> :<message> : Sends a private message to a user.\n"
    		"JOIN <channel> : Joins the specified channel.\n"
    		"USER <username> : Sets the username for the client.\n"
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

	namespace server {

	}
}

#endif
