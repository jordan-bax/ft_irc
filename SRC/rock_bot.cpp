#include "../HEADER/rock_bot.hpp"
#include "../HEADER/mycolor.hpp"
#include "../HEADER/error_log.hpp"
#include "../HEADER/hand_sign.hpp"
#include "../HEADER/server_exception.hpp"
#include <cstring>
#include <algorithm>

rock_bot::rock_bot(): client(FD_CLIENT, 0,"0",0) {
	std::cout << "new rockbot [" << _fd << "]"<< std::endl;
	_user = new User_data(_fd);
	_user->set_nickname("Rockbot");
	_user->set_fullname("RockBot");
	_user->set_username("RockBot");
}
rock_bot::rock_bot( env &e ) : client(FD_CLIENT, 1,"0",0){
	std::cout << "new rockbot [" << _fd << "]"<< std::endl;
	_user = new User_data(_fd);
	_user->set_nickname("rockbot");
	_user->set_fullname("RockBot");
	_user->set_username("RockBot");
	_authorised=true;
	_server_info = &e;
	_art = false;
}
rock_bot::rock_bot( rock_bot const & src ) : client(FD_CLIENT, 0,"0",0){
	std::cout << "new client [" << _fd << "]"<< std::endl;
}
rock_bot::~rock_bot() {
	std::cout << "del rockbot [" << _fd << "]"<< std::endl;
}

rock_bot & rock_bot::operator=( rock_bot const & rhs ) {
	return *this;
}

static std::string	get_line(std::string &tekst, char c) {
	std::string line;
	size_t		pos = tekst.find(c);
	if (!pos){
		line = tekst;
		tekst.clear();
		tekst.shrink_to_fit();
		return line;
	}
	line = tekst.substr(0, pos + 1);
	tekst = tekst.substr(pos + 1);
	return line;
}
static size_t min(size_t a, size_t b){
	return (a<b)? a:b;
}
static std::string get_send_location(std::string buf) {
    std::string target;
    size_t pos = buf.find('#');
    if (pos == std::string::npos) {
        pos = min(buf.find('!'), buf.find(' '));
        if (pos == std::string::npos) {
            return ""; // Return empty string if no valid position is found
        }
        target = buf.substr(1, pos - 1);
        return target;
    }
    size_t end_pos = buf.find_first_of(" :\t\n\0", pos);
    if (end_pos == std::string::npos) {
        end_pos = buf.length();
    }
    target = buf.substr(pos, end_pos - pos);
    return target;
}

static int	get_play(std::string buf, bool art = false){
	size_t pos = buf.find_last_of(':');
	std::string play = buf.substr(pos);
	if (play.find("art") != std::string::npos)
		return -1;
	for (size_t i = 0; g_hand_sign[i]; i++)
	{
		if (play.find(g_hand_sign[i]) != std::string::npos)
			return i;
	}
	return 3;
}
static std::string	get_handSign(int token, bool art = false){
	std::string sign;
	if (token > 2 || token < 0)
		return "error";
	if (art == true){
		sign = g_hand_sign[token];
		sign += "\n";
		sign += g_hand_sign_art[token];
		return sign;
	}
	return g_hand_sign[token];
}
static int	random_handSign(void){
	static bool seed = false;
	if (seed == false){
		srand(time(NULL));
		seed = true;
	}
    return rand() % 3;
}
static std::string who_wins(int player , int bot){
	if (player == bot)
		return "it's a draw\n";
	if	(player == paper && bot == rock)
		return FG_GREEN"you WON!!" FG_DEFAULT;
	if	(player == rock && bot == scissor)
		return FG_GREEN"you WON!!" FG_DEFAULT;
	if	(player == scissor && bot == paper)
		return FG_GREEN"you WON!!" FG_DEFAULT;
	return FG_RED"you LOSE!!" FG_DEFAULT;
}

bool	rock_bot::join(std::string buf){
	std::string line = "JOIN " + get_send_location(buf);
	buf_read = line ;//+ "rock bot can not join the channel\n";
	if (try_send())
	{
		buf_read.clear();
		return true;
	}
	size_t pos = min(buf.find('!'), buf.find(' '));
	if (pos == std::string::npos || pos == 0) 
	{
		std::cerr << "Invalid position in buffer" << std::endl;
		return false;
	}
	std::string target = buf.substr(1,pos -1);
	line = "PRIVMSG " + target + " :";
	buf_read = line + "rock bot can not join the channel\n";
	try_send();
	buf_read.clear();
	buf.clear();
	return false;
}
bool	rock_bot::try_send(void){
	try {
		handle_client_input(*this->_server_info);
	}
	catch(const server_exception& e) {
		std::cout << e.what();
		return false;
	}
	catch(const client_exception& e) {
		send_numeric_reply(*this->_server_info, e);
		return false;
	}
	return true;
}
static void remove_char(std::string &str, char c) {
	str.erase(std::remove(str.begin(), str.end(), c), str.end());
}

void	rock_bot::write(void){
	std::string buf = get_line(buf_write,'\n');
	remove_char(buf, '\r');
	if (buf.find("INVITE") != std::string::npos)
	{
		join(buf);
		return;
	}
	if (buf.find("PRIVMSG") == std::string::npos)
	{
		buf.clear();
		return;
	}
	std::string line = "PRIVMSG " + get_send_location(buf) + " :";
	int player = get_play(buf);
	int bot = random_handSign();
	if (player < 0){
		if (_art == true){
			_art = false;
			buf_read = line + "rock bot will not use ascii art\n";
		}
		else{
			_art = true;
			buf_read = line + "rock bot will use ascii art\n";
		}

	}
	else if (player > 2)
		buf_read = line + "rock bot does not know\n";
	else{

		buf_read = line + "you played \n" + get_handSign(player, _art);
		try_send();
		buf_read = line + "rockbot played \n" + get_handSign(bot, _art);
		try_send();
		buf_read = line + who_wins(player, bot) + "\n";
	}
	try_send();
	buf_read.clear();
	buf.clear();
	// this->buf_write.clear();
}

bool	rock_bot::read(env &server_env){
	return true;
}