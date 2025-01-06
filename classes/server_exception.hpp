#ifndef SERVER_EXCEPTION_HPP
#define SERVER_EXCEPTION_HPP

#include <stdexcept>

class server_exception: public std::exception {
	private:
		std::string	msg;

	public:
		explicit server_exception(std::string const &msg): msg(msg) {}
		const char *what() const noexcept override {
			return (msg.c_str());
		}
};

#endif
