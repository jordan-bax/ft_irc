#pragma once
#include "connection.hpp"

class client : public connection
{
private:
	std::string _full_name;
	std::string _nick_name;
	std::string _password;

	bool	commands();
	void	handle_client_input();
public:
	client();
	client( int type , int fd );
	// client( client const & src );
	~client();

	// client & operator=( client const & rhs );
	void	write(void);
	bool	read(s_env *env);
};

// std::ostream & operator<<( std::ostream & o, client const & rhs);
