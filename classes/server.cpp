#include "server.hpp"
#include "client.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../preset/bircd.h"

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
void	server::read(s_env *env) {
	int			cs;
	struct sockaddr_in	csin;
	socklen_t		csin_len;

	csin_len = sizeof(csin);
	cs = X(-1, accept(this->_fd, (struct sockaddr*)&csin, &csin_len), "accept");
	std::cout << "New client #"<<cs <<" from " <<inet_ntoa(csin.sin_addr)<<":"<< ntohs(csin.sin_port)<< std::endl; 
	// printf("New client #%d from %s:%d\n", cs,
	// 	inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));
	env->connections.push_back(new client(FD_CLIENT, cs));
	env->connections.back()->set_bufwrite(WELCOME);
	// clean_fd(&e->fds[cs]);
	// e->fds[cs].type = FD_CLIENT;
	// e->fds[cs].fct_read = client_read;
	// e->fds[cs].fct_write = client_write;
	// e->fds[cs].user = new User_data(cs);
	// e->fds[cs].buf_write = WELCOME;
}

// server & server::operator=( server const & rhs ) {
	
// }
// std::ostream & operator<<( std::ostream & o, server const & rhs) {
// 	// o << rhs.getName();
// 	return o;
// }
