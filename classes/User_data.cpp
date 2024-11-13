#include "User_data.hpp"

User_data::User_data() : _fd(-1) ,_full_name("unkown"),_nick_name("unkown"), _password("") {
	std::cout << "new user [" << _fd << "]\n";
}
User_data::User_data( int fd ) : _fd(fd) ,_full_name("unkown"),_nick_name("unkown"), _password("") {
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
	this->_nick_name = rhs._nick_name;
	this->_password = rhs._password;
	return *this;
}
bool	User_data::set_fullname(std::string name){
	_full_name = name;
	// check file for existing name
	return false;
}
void	User_data::set_nickname(std::string name){
	_nick_name = name;
}
void	User_data::set_password(std::string password){
	_password = password;
}
bool	User_data::check_password(std::string password) const{
	return password == _password ? true :false;
}
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
