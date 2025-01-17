#ifndef CLIENT_EXCEPTION_HPP
#define CLIENT_EXCEPTION_HPP

#include "../Messages.hpp"
#include <stdexcept>
#include <vector>

class client_exception: public std::exception {
	private:
		std::vector<std::string>	_params;
		messages::Client			_numeric_reply;

	public:
		explicit client_exception(messages::Client numeric_reply, std::vector<std::string> params): _numeric_reply(numeric_reply), _params(params) {}
		explicit client_exception(messages::Client numeric_reply): _numeric_reply(numeric_reply) {}

		messages::Client const	&get_numeric_reply() const { return _numeric_reply; }
		std::vector<std::string> const	&get_params() const { return _params; }
};

#endif
