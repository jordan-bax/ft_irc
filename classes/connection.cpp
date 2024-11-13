#include "connection.hpp"
#include <unistd.h>

connection::connection() {
	
}
connection::connection( int type, int fd ): _fd(fd),_type(type) {
	std::cout << "new connection [" << _fd << "]\n";

}
connection::~connection() {
	close(_fd);
	std::cout << "delete connection [" << _fd << "]\n";

}

// connection::connection( connection const & src ) {
	
// }

// connection & connection::operator=( connection const & rhs ) {
	
// }
// std::ostream & operator<<( std::ostream & o, connection const & rhs) {
// 	o << rhs.getName();
// 	return o;
// }
