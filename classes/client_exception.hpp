#ifndef CLIENT_EXCEPTION_HPP
#define CLIENT_EXCEPTION_HPP

#include "../Messages.hpp"
#include <stdexcept>

class client_exception: public std::exception {
	private:
		std::string			param;
		messages::Client	numeric_reply;

	public:
		explicit client_exception(messages::Client numeric_reply, std::string param): numeric_reply(numeric_reply), param(param) {}
		explicit client_exception(messages::Client numeric_reply): numeric_reply(numeric_reply), param("") {}
		
		const char *what() const noexcept override {
			return (param.c_str());
		}

		messages::Client get_numeric_reply() const {
        	return numeric_reply;
    	}
};

#endif
