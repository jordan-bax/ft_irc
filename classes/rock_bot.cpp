#include "rock_bot.hpp"
#include "../mycolor.hpp"
#include "../other/error_log.hpp"
#include "../headers/hand_sign.hpp"
#include <cstring>

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
static std::string	get_send_location(std::string buf){
	
	std::string target;
	size_t		pos = buf.find('#');
	if (pos == std::string::npos){
		pos = min(buf.find('!'), buf.find(' '));
		target = buf.substr(1,pos);
		return target;
	}
	target = buf.substr(pos,buf.find(' ', pos));
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
void	rock_bot::write(void){
	std::string buf= get_line(buf_write,'\n');
	log(buf);
	// if (buf.find("INVITE") == std::string::npos)
	if (buf.find("PRIVMSG") == std::string::npos)
	{
		buf.clear();
		return;
	}
	std::string line = "PRIVMSG " + get_send_location(buf) + " :";
	int player = get_play(buf);
	int bot = random_handSign();
	if (player < 0){
		_art = true;
		buf_read = line + "rock bot will use ascii art\n";
	}
	else if (player > 2)
		buf_read = line + "rock bot does not know\n";
	else{

		buf_read = line + "you played \n" + get_handSign(player, _art);
		handle_client_input(*this->_server_info);
		buf_read = line + "rockbot played \n" + get_handSign(bot, _art);
		handle_client_input(*this->_server_info);
		buf_read = line + who_wins(player, bot) + "\n";
		
	}
	handle_client_input(*this->_server_info);
	buf_read.clear();
	buf.clear();
	// this->buf_write.clear();
}
/*
PASS password
NICK jor
USER jor  * *: roro
PRIVMSG Rockbot : hoi
*/
bool	rock_bot::read(env &server_env){
	// int		i;
	// int		r;
	// char	buf_read[BUF_SIZE + 1];

	// r = recv(this->_fd, buf_read, BUF_SIZE, 0);
	// if (r <= 0)
	// {
	// 	std::cout << "client #" << this->_fd << "gone away"<< std::endl;
	// 	close(this->_fd);
	// 	return false;
	// }
	// buf_read[r] = '\0';
	// std::cout << "get read" <<" > "<< buf_read << " read\n";
	// this->buf_read = buf_read;
	// try {
	// 	std::vector<std::string> ss = this->split(this->buf_read, '\n');
	// 	for (auto pp : ss)
	// 	{
	// 		if (pp.back()== '\r')
	// 			pp.pop_back();
	// 		this->buf_read = pp + '\n';
	// 		std::cout << this->buf_read << ss.size()<< std::endl;
	// 		// print_ascii(this->buf_read);
	// 		// print_ascii(pp);
	// 		// print_ascii(ss[0]);
	// 		handle_client_input(server_env);
	// 	}
	// }
	// catch(const server_exception& e) {
	// 	std::cout << e.what();
	// }
	// catch(const client_exception& e) {
	// 	send_numeric_reply(server_env, e);
	// }
	
	return true;
}