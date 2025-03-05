#pragma once
#include <iostream>
#include "client.hpp"

class rock_bot : public client 
{
private:
	bool 	_art;
	env		*_server_info;
	rock_bot();
	bool 	try_send(void);
	bool	join(std::string buf);

public:
	rock_bot( env &e );
	rock_bot( rock_bot const & src );
	~rock_bot();

	void	write(void);
	bool	read(env &server_env);
	rock_bot & operator=( rock_bot const & rhs );

};

