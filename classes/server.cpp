#include "server.hpp"
#include "client.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../preset/bircd.h"
#include "../other/error_log.hpp"

server::server() {
	
}
server::server( int type,int fd ): connection(type, fd) {
	std::cout << "new server [" << _fd << "]\n";
}
// server::server( server const & src ) {
	
// }
server::~server() {
	std::cout << "del server [" << _fd << "]\n";
}

void	server::write(void) {
	std::cout << "server write test [" << _fd << "]\n";

}
bool	server::read(env &server_env) {
	int			client_fd;
	struct sockaddr_in	csin;
	socklen_t		csin_len;
	std::string		client_adderess;
	uint16_t		client_port;

	if (server_env.get_connections().size() >= FD_SETSIZE)
		return true;
	csin_len = sizeof(csin);
	client_fd = err_int(-1, accept(this->_fd, (struct sockaddr*)&csin, &csin_len), "accept");
	client_adderess = inet_ntoa(csin.sin_addr);
	client_port =  ntohs(csin.sin_port);
	std::cout << "New client #"<< client_fd <<" from " << client_adderess << ":"<< client_port << std::endl; 
	server_env.get_connections().push_back(new client(FD_CLIENT, client_fd, client_adderess, client_port));
	//server_env.get_connections().back()->set_bufwrite("WELCOME");
	return true;
}

// server & server::operator=( server const & rhs ) {
	
// }
// std::ostream & operator<<( std::ostream & o, server const & rhs) {
// 	// o << rhs.getName();
// 	return o;
// }
