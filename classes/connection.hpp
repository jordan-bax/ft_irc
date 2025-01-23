#pragma once
#include <iostream>
#include "../preset/bircd.h"

struct env;

class connection
{
private:

protected:
	int		_fd;
	int		_type;
	std::string	buf_read;
	std::string	buf_write;

public:
	connection();
	connection( int type , int fd);
	// connection( connection const & src );
	virtual ~connection();

	// connection & operator=( connection const & rhs );
	static std::vector<connection*>::iterator find(std::vector<connection*>::iterator start, std::vector<connection*>::iterator end, int fd)
	{
		while (start != end)
		{
			if ((*start)->_fd == fd)
				return start;
			start++;
		}
		return start;
	};
	static void		clear(std::vector<connection*> &start)
	{
		while (!start.empty())
		{
			delete start.back();
			start.pop_back();
		}
		start.shrink_to_fit();
	};
	void			set_bufwrite(std::string write){this->buf_write = write;};
	std::string		get_bufwrite()const{return buf_write;};
	int				get_fd()const{return _fd;};
	int				get_type()const{return _type;};
	virtual bool	read(env &server_env) = 0;
	virtual void	write(void) = 0;
};

// std::ostream & operator<<( std::ostream & o, connection const & rhs);
