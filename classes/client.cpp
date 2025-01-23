#include "client.hpp"
#include "client_exception.hpp"
#include "server_exception.hpp"
#include <sys/socket.h>
#include <unistd.h>

client::client() {
	std::cout << "new client [" << _fd << "]\n";
	
}
client::client( int type, int fd ) : connection(type, fd) {
	std::cout << "new client [" << _fd << "]\n";
	
}
// client::client( client const & src ) {
// 	std::cout << "new client [" << _fd << "]\n";
	
// }
client::~client() {
	for (auto &channel: _channels) {
		channel->remove_client(_user->get_nickname());
	}
	if (_user)
		delete(_user);
	std::cout << "del client [" << _fd << "]\n";
}
void	client::write(void){
	send(this->_fd, this->buf_write.c_str(), this->buf_write.length(), 0);
	this->buf_write.clear();
}

// bool client::commands(){
// 	if (this->buf_read.length() > 6 && !this->buf_read.compare(0, 6, "LOGIN "))
// 	{
// 		std::string temp(this->buf_read.substr(6,this->buf_read.size()));
// 		temp.pop_back();
// 		this->_full_name = temp;
// 		this->_nick_name = temp;
// 		return false;
// 	}
// 	return true;
// }
void print_ascii(std::string s){for (int i: s){std::cout << i << " " << static_cast<char>(i) << " ";}std::cout <<'\n';}

bool	client::read(env &server_env){
	int		i;
	int		r;
	char	buf_read[BUF_SIZE + 1];

	r = recv(this->_fd, buf_read, BUF_SIZE, 0);
	if (r <= 0)
	{
		std::cout << "client #" << this->_fd << "gone away"<< std::endl;
		close(this->_fd);
		return false;
	}
	buf_read[r] = '\0';
	std::cout << "get read" <<" > "<< buf_read << " read\n";
	this->buf_read = buf_read;
	try {
		std::vector<std::string> ss = this->split(this->buf_read, '\n');
		for (auto pp : ss)
		{
			if (pp.back()== '\r')
				pp.pop_back();
			this->buf_read = pp + '\n';
			std::cout << this->buf_read << ss.size()<< std::endl;
			// print_ascii(this->buf_read);
			// print_ascii(pp);
			// print_ascii(ss[0]);
			handle_client_input(server_env);
		}
	}
	catch(const server_exception& e) {
		std::cout << e.what();
	}
	catch(const client_exception& e) {
		send_numeric_reply(e);
	}
	
	return true;
}
