#pragma once
#include <iostream>

class User_data
{
private:
	int			_fd;
	std::string _full_name;
	std::string _nick_name;
	std::string _password;

public:
	User_data();
	User_data( int fd );
	User_data( User_data const & src );
	~User_data();

	bool set_fullname(std::string name);
	void set_nickname(std::string name);
	void set_password(std::string password);
	bool check_password(std::string password) const;

	std::string massege(std::string buffer);
	User_data & operator=( User_data const & rhs );

};

// std::ostream & operator<<( std::ostream & o, User_data const & rhs);
