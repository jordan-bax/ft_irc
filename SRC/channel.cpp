#include "../HEADER/channel.hpp"
#include <algorithm>
#include <iostream>
#include "../HEADER/client.hpp"

channel::channel() {}

channel::channel(std::string name) : _name(name), _invite_only(false), _topic_permision(false), _limit(DEFAULT_LIMIT) {
    std::cout << "new channel [" << _name << "]\n";
}

channel::channel(channel const &src) {
    *this = src;
}

channel::~channel() {
    std::cout << "delete channel [" << _name << "]\n";
}

channel &channel::operator=(channel const &rhs) {
    _name = rhs._name;
    _password = rhs._password;
    _invite_only = rhs._invite_only;
    _topic_permision = rhs._topic_permision;
    _topic_msg = rhs._topic_msg;
    _limit = rhs._limit;
    _clients = rhs._clients;
    _operators = rhs._operators;
    return (*this);
}

void channel::clear(std::vector<channel*> &start) {
    while (!start.empty()) {
        delete start.back();
        start.pop_back();
    }
    start.shrink_to_fit();
}

void channel::add_client(client *client) {
    if (client != NULL)
        _clients.push_back(client);
}

void channel::remove_client(std::string nick_name) {
    remove_invite(nick_name);
    for (int i = 0; i < _clients.size(); i++) {
        if (_clients[i]->get_nick() == nick_name)
            _clients.erase(_clients.begin() + i);
    }
}

void channel::add_operator(std::string user_name) {
    _operators.push_back(user_name);
}

void channel::remove_operator(std::string user_name) {
    for (int i = 0; i < _operators.size(); i++) {
        if (_operators[i] == user_name)
            _operators.erase(_operators.begin() + i);
    }
}

void channel::add_invite(std::string user_name) {
    _invites.push_back(user_name);
}

void channel::remove_invite(std::string name) {
    for (int i = 0; i < _invites.size(); i++) {
        if (_invites[i] == name)
            _invites.erase(_invites.begin() + i);
    }
}

bool channel::user_in_channel(std::string name) {
    for (client *client : _clients) {
        if (client->get_nick() == name)
            return (true);
    }
    return (false);
}

bool channel::user_is_operator(std::string name) {
    for (std::string op : _operators) {
        if (op == name)
            return (true);
    }
    return (false);
}

bool channel::user_is_invited(std::string name) {
    for (std::string user : _invites) {
        if (user == name)
            return (true);
    }
    return (false);
}

bool channel::check_key(std::string const key) {
    return (key == _password);
}

void channel::clear_topic() {
    _topic_msg.clear();
}

bool const channel::is_full() const {
    if (_limit == 0)
        return (false);
    if (_clients.size() >= _limit)
        return (true);
    return (false);
}

void channel::set_key(std::string key) {
    _password = key;
}

void channel::set_topic(std::string const topic) {
    _topic_msg = topic;
}

void channel::set_topic_permission(bool permission) {
    _topic_permision = permission;
}

void channel::set_invonly(bool invonly) {
    _invite_only = invonly;
}

void channel::set_limit(unsigned int limit) {
    _limit = limit;
}

std::string const &channel::get_name() const {
    return (_name);
}

std::string const &channel::get_topic() const {
    return (_topic_msg);
}

bool const channel::get_topic_permission() const {
    return (_topic_permision);
}

bool const &channel::get_invonly() const {
    return (_invite_only);
}
unsigned int		channel::get_user_count() const {
	return (_clients.size());
}
void channel::send_message(env const &env, User_data const &sender, std::string const &sender_nick, std::string const &msg) {
    for (auto *client : _clients) {
        if (client->get_nick() != sender_nick)
            client->recieve_channel_message(env, sender, _name, msg);
    }
}

void channel::send_mode_message(env const &env, User_data const &sender, std::string const &cmd, std::string const &msg) {
    for (auto *client : _clients) {
        client->send_mode_message(env, sender, get_name(), cmd, msg);
    }
}

bool channel::valid_name(std::string name) {
    if (name.empty() || name.length() > 200)
        return (false);
    if (std::string("#&+!").find(name[0]) == std::string::npos)
        return (false);
    return std::all_of(name.begin() + 1, name.end(), [](char c) {
        return (std::isalnum(c) || c == '-' || c == '_' || c == '.');
    });
}
