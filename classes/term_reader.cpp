#include "term_reader.hpp"
#include <sys/socket.h>
#include "env.hpp"

term_reader::term_reader() {
	
}
term_reader::term_reader( int type,int fd ): connection(type, fd) {
	std::cout << "new reader [" << _fd << "]\n";
	
}
term_reader::term_reader( term_reader const & src ) {
	
}
term_reader::~term_reader() {
	std::cout << "del reader [" << _fd << "]\n";
}

term_reader & term_reader::operator=( term_reader const & rhs ) {
	return *this;
}
// std::ostream & operator<<( std::ostream & o, term_reader const & rhs) {
// 	o << rhs.getName();
// 	return o;
// }
void	term_reader::write(void) {}
bool	term_reader::read(env &server_env) {
	int	r;
	int	i;
	// std::string buf;
	std::getline(std::cin, this->buf_read, '\n');
	// e->fds[cs].buf_read = buf;
	this->buf_read.append("\n");
	// e->fds[cs].buf_read[buf.size()] = '\0';
	if (this->buf_read.size())
	{
		i = 0;
		while (server_env.get_connections().size() > i)
		{
			if ((server_env.get_connections()[i]->get_type() == FD_CLIENT) )
			{
				// std::string message = e->fds[cs].user->massege(e->fds[cs].buf_read);
				// std::cout <<"massage> " <<message << "size>" << message.size()<< std::endl;
				// send(server_env.get_connections()[i]->get_fd(), "ADMIN; ",8 , 0);
				send(server_env.get_connections()[i]->get_fd(), this->buf_read.c_str(),this->buf_read.size() , 0);
			}
			i++;
		}
	}
	return true;

}