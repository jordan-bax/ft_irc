#pragma once
#include "connection.hpp"

class server : public connection
{
private:
	server();

public:
	server( int type , int fd );
	// server( server const & src );
	~server();

	// server & operator=( server const & rhs );
	void	write(void);
	bool	read(env &server_env);
};

// std::ostream & operator<<( std::ostream & o, server const & rhs);
