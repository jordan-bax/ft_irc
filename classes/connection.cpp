#include "connection.hpp"
#include <unistd.h>

connection::connection() : _fd(-1), _type(-1) {}

connection::connection(int type, int fd) : _fd(fd), _type(type) {
	std::cout << "new connection [" << _fd << "]\n";
}

connection::~connection() {
	if (_fd > 2)
		close(_fd);
	std::cout << "delete connection [" << _fd << "]\n";
}

std::vector<connection*>::iterator connection::find(std::vector<connection*>::iterator start, std::vector<connection*>::iterator end, int fd) {
    while (start != end) {
        if ((*start)->_fd == fd)
            return start;
        start++;
    }
    return start;
}

void connection::clear(std::vector<connection*> &start) {
    while (!start.empty()) {
        delete start.back();
        start.pop_back();
    }
    start.shrink_to_fit();
}

void connection::set_bufwrite(std::string write) {
    this->buf_write = write;
}

std::string connection::get_bufwrite() const {
    return buf_write;
}

int connection::get_fd() const {
    return _fd;
}

int connection::get_type() const {
    return _type;
}
