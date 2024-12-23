#pragma once
#include "connection.hpp"

class term_reader : public connection
{
private:
	term_reader();
	term_reader( term_reader const & src );
	term_reader & operator=( term_reader const & rhs );
	//variable

public:
	term_reader( int type , int fd  );
	~term_reader();

	void	write(void);
	bool	read(s_env *env);
	
};

std::ostream & operator<<( std::ostream & o, term_reader const & rhs);
