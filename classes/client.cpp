#include "client.hpp"
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
	std::cout << "del client [" << _fd << "]\n";
	
}
void	client::write(void){
	send(this->_fd, this->buf_write.c_str(), this->buf_write.length(), 0);
	this->buf_write.clear();
}

bool client::commands(){
	if (this->buf_read.length() > 6 && !this->buf_read.compare(0, 6, "LOGIN "))
	{
		std::string temp(this->buf_read.substr(6,this->buf_read.size()));
		temp.pop_back();
		this->_full_name = temp;
		this->_nick_name = temp;
		return false;
	}
	return true;
}

bool	client::read(s_env *env){
	int		i;
	int		r;
	char	buf_read[BUF_SIZE + 1];

	r = recv(this->_fd, buf_read, BUF_SIZE, 0);
	if (r <= 0)
	{
		std::cout << "client #" << this->_fd << "gone away"<< std::endl;
		close(this->_fd);
		// std::vector<connection*>::iterator it = connection::find(env->connections.begin(), env->connections.end(), this->_fd);
		// delete (*it);
		// it = 0;
		// env->connections.erase(it);
		return false;
	}
	buf_read[r] = '\0';
	std::cout << "get read" <<" > "<< buf_read << " read\n";
	this->buf_read = buf_read;
	if (commands())
	{
		std::vector<connection*>::const_iterator it = env->connections.cbegin();
		std::vector<connection*>::const_iterator ite = env->connections.cend();
		while (it != ite)
		{
			if (((*it)->get_type() == FD_CLIENT) && (this->_fd != (*it)->get_fd()))
			{
				std::string message = _nick_name + " " + this->buf_read;
				std::cout <<"massage> " <<message << "size>" << message.size()<< std::endl;
				send((*it)->get_fd(), message.c_str(),message.size() , 0);
			}
			it++;
		}
	}
	return true;
}

// client & client::operator=( client const & rhs ) {
	
// }
// std::ostream & operator<<( std::ostream & o, client const & rhs) {
// 	o << rhs.getName();
// 	return o;
// }
