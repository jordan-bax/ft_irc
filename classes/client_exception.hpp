#ifndef CLIENT_EXCEPTION_HPP
#define CLIENT_EXCEPTION_HPP

#include <stdexcept>

class client_exception: public std::exception {
	private:
		std::string	msg;

	public:
		explicit client_exception(std::string const &msg): msg(msg) {}
		const char *what() const noexcept override {
			return (msg.c_str());
		}
};

#endif
