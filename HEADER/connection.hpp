#pragma once
#include <iostream>
#include <vector>

class env;

class connection
{
private:

protected:
	int		_fd;
	int		_type;
	std::string	buf_read;
	std::string	buf_write;

	connection();
public:
	connection( int type , int fd);
	virtual ~connection();

	static std::vector<connection*>::iterator find(std::vector<connection*>::iterator start, std::vector<connection*>::iterator end, int fd);
	static void		clear(std::vector<connection*> &start);
	void			set_bufwrite(std::string write);
	std::string		get_bufwrite()const;
	int				get_fd()const;
	int				get_type()const;
	virtual bool	read(env &server_env) = 0;
	virtual void	write(void) = 0;
};
