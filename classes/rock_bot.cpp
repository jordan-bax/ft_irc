#include "rock_bot.hpp"
#include "../mycolor.hpp"
#include "../other/error_log.hpp"

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
	_user->set_nickname("Rockbot");
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

void	rock_bot::write(void){

	std::string line = get_line(this->buf_write, '\n');
	if (line[0] == ':')
		line.erase(0,1);
		log(line);
	buf_read = "PRIVMSG " + get_line(line, ' ');
	get_line(line, ':') ;
	buf_read += ":"+ get_line(line, '\n');

	// if (buf_write.length()>0)
		log(buf_read);
	
	handle_client_input(*this->_server_info);
	buf_read.clear();
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