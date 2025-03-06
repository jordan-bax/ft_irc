#include "../HEADER/User_data.hpp"

User_data::User_data() : _fd(-1) ,_full_name("unkown"),_nick_name("unkown") {
	std::cout << "new user [" << _fd << "]\n";
}
User_data::User_data( int fd ) : _fd(fd) ,_full_name("unkown"),_nick_name("unkown") {
	std::cout << "new user [" << _fd << "]\n";
}
User_data::User_data( User_data const & src ) {
	*this = src;
}
User_data::~User_data() {
	
	std::cout << "delete user [" << _fd << "]\n";
}

User_data & User_data::operator=( User_data const & rhs ) {
	this->_fd = rhs._fd;
	this->_full_name = rhs._full_name;
	this->_user_name = rhs._user_name;
	this->_nick_name = rhs._nick_name;
	return *this;
}
bool	User_data::set_fullname(std::string name){
	_full_name = name;
	// check file for existing name
	return false;
}

void	User_data::set_username(std::string name) {
	_user_name = name;
}

void	User_data::set_nickname(std::string name){
	_nick_name = name;
}

std::string const	&User_data::get_fullname() const { return (_full_name); }
std::string const	&User_data::get_username() const { return (_user_name); }
std::string	const	&User_data::get_nickname() const { return (_nick_name); }

std::string User_data::massege(std::string buffer){
	std::string out;
	out = _nick_name + " " + buffer;
	std::cout<< _nick_name<< std::endl;
	return out;
}
// std::ostream & operator<<( std::ostream & o, User_data const & rhs) {
// 	o << rhs.getName();
// 	return o;
// }
