#include "../HEADER/client.hpp"
#include "../HEADER/client_exception.hpp"
#include "../HEADER/server_exception.hpp"
#include <sys/socket.h>
#include <unistd.h>

# define BUF_SIZE	4096

client::client() {
	std::cout << "new client [" << _fd << "]"<< std::endl;
	
}
client::client( int type, int fd ) : connection(type, fd) {
	std::cout << "new client [" << _fd << "]"<< std::endl;
	
}
client::client( int type , int fd , std::string addr, uint16_t port) : connection(type, fd), _address(addr), _port(port) {
	std::cout << "new client [" << _fd << "]"<< this->_address<< std::endl;
	
}
// client::client( client const & src ) {
// 	std::cout << "new client [" << _fd << "]"<< std::endl;
	
// }
client::~client() {
	for (auto &channel: _channels) {
		channel->remove_client(_user->get_nickname());
	}
	if (_user)
		delete(_user);
	std::cout << "del client [" << _fd << "]"<< std::endl;
}
static std::string	get_line(std::string &tekst) {
	std::string line;
	size_t		pos = tekst.find('\n');
	if (!pos){
		line = tekst;
		tekst.clear();
		return line;
	}
	line = tekst.substr(0, pos + 1);
	tekst = tekst.substr(pos + 1);
	return line;
}
void	client::write(void){
	std::string line = get_line(this->buf_write);
	send(this->_fd, line.c_str(), line.length(), 0);
	// this->buf_write.clear();
}

static void print_ascii(std::string s){for (int i: s){std::cout << i << " " << static_cast<char>(i) << " ";}std::cout <<'\n';}

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
	std::vector<std::string> ss = this->split(this->buf_read, '\n');
	for (auto pp : ss)
	{
		if (pp.back()== '\r')
			pp.pop_back();
		this->buf_read = pp + '\n';
		try {
			handle_client_input(server_env);
		}
		catch(const server_exception& e) {
			std::cout << e.what();
		}
		catch(const client_exception& e) {
			send_numeric_reply(server_env, e);
		}
	}
	
	return true;
}

std::string	const &client::get_nick() const {
	return (_user->get_nickname());
}
std::string const &client::get_usrname() const {
	return (_user->get_username());
}
bool	client::is_registered() const {
	return (_user != NULL);
}