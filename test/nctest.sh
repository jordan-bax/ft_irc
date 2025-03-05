# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    nctest.sh                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbax <jbax@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/19 20:25:57 by ohengelm          #+#    #+#              #
#    Updated: 2025/03/05 17:28:42 by jbax             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#! /bin/bash

if [ "$(uname)" = "Darwin" ]; then
	IRC_SERVER=$(ifconfig | grep "inet " | grep -v 127.0.0.1 | awk '{print $2}')
else
	IRC_SERVER=$(hostname -I | awk '{print $1}')
fi
PORT=8000
PASSWORD="password"
USER1="user$1"
USER2="full$1"
NICK="nick$1"
CHANNEL="#ncTestChannel"
{
	sleep 0.1
# Connection requirements
	echo > /dev/tty;
	echo "PASS $PASSWORD";
	echo "NICK $NICK";
	echo "USER $USER1 * $IRC_SERVER :$USER2";
	sleep 0.1;
# Hardcoded tests
	echo "PRIVMSG $NICK :Hello me"
	echo "JOIN $CHANNEL"
	echo "INVITE rockbot $CHANNEL"
	sleep 0.4
	echo "PRIVMSG $CHANNEL :rock"
	sleep 0.4
	echo "KICK $CHANNEL rockbot :I'm crazy with power!"
	sleep 0.1
	echo "MODE $CHANNEL"
	echo "MODE $CHANNEL +l 1"
	echo "MODE $CHANNEL +i"
	echo "MODE $CHANNEL +t"
	# echo "MODE $CHANNEL +k SabotagingPassword"
	echo "MODE $CHANNEL -o $NICK"
	echo "MODE $CHANNEL"
	sleep 0.1
	echo "PART $CHANNEL"
	# sleep 0.1
	# echo "PRIVMSG rockbot ://rock"
	# sleep 0.1;
	# echo "PRIVMSG rockbot ://paper"
	# echo "PRIVMSG rockbot ://art"
	# echo "PRIVMSG rockbot ://rock"
# Endof harcoded tests
	sleep 0.1;
	echo > /dev/tty;
	while [ true ]; do
		read -p "nc (or quit): " COMMAND;
		if [ "$COMMAND" = "quit" ]; then
			echo "QUIT :leaving...";
			break ;
		else
			echo "$COMMAND";
		fi
		sleep 0.1;
		echo > /dev/tty
	done
} | tee /dev/tty | nc $IRC_SERVER $PORT
