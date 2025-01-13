#pragma once
#include <iostream>

class client;

class User_data
{
private:
	int			_fd;
	std::string _full_name;
	std::string	_user_name;
	std::string _nick_name;

public:
	User_data();
	User_data( int fd );
	User_data( User_data const & src );
	~User_data();

	bool	set_fullname(std::string name);
	void	set_username(std::string name);
	void	set_nickname(std::string name);

	std::string const	&get_fullname() const;
	std::string	const	&get_username() const;
	std::string	const	&get_nickname() const;

	std::string massege(std::string buffer);
	User_data & operator=( User_data const & rhs );

};

// std::ostream & operator<<( std::ostream & o, User_data const & rhs);
