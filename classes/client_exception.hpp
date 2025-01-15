#ifndef CLIENT_EXCEPTION_HPP
#define CLIENT_EXCEPTION_HPP

#include "../Messages.hpp"
#include <stdexcept>

class client_exception: public std::exception {
	private:
		std::string			_p1;
		std::string			_p2;
		messages::Client	_numeric_reply;

	public:
		explicit client_exception(messages::Client numeric_reply, std::string p1, std::string p2): _numeric_reply(numeric_reply), _p1(p1), _p2(p2) {}
		explicit client_exception(messages::Client numeric_reply, std::string param): _numeric_reply(numeric_reply), _p1(param) {}
		explicit client_exception(messages::Client numeric_reply): _numeric_reply(numeric_reply) {}

		messages::Client const	&get_numeric_reply() const { return _numeric_reply; }
		std::string const 		&get_p1() const { return _p1; }
		std::string const 		&get_p2() const { return _p2; }
};

#endif
