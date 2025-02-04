#include "env.hpp"
#include <sys/resource.h>
#include <unistd.h>
#include "server_exception.hpp"
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include "../classes/server.hpp"
#include "../classes/term_reader.hpp"
#include "../classes/client_exception.hpp"
#include "../Messages.hpp"
#include "client.hpp"
#include "channel.hpp"
#include <chrono>
#include "../other/error_log.hpp"

env::env() {
	time_t date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	this->_date = ctime(&date);
	std::cout << "env made" << std::endl;
}

env::env( env const & src ) {
	*this = src;
	std::cout << "env made" << std::endl;
}

env::~env() {
	std::cout << "env removed" << std::endl;
}

env & env::operator=( env const & rhs ) {
	this->connections = rhs.connections;
	this->channels = rhs.channels;
	this->_hostname = rhs._hostname; // hostname of the computer
	this->_port = rhs._port;// the port to lissen
	this->_maxfd = rhs._maxfd;// set resource soft limits
	this->_max = rhs._max; // the biggest fd that select will search
	this->_select = rhs._select;// select returned 
	this->_fd_read = rhs._fd_read;// a select set with the fd to read in
	this->_fd_write = rhs._fd_write;
	return *this;
}
bool	env::set_port(std::string port) {
	try
	{
		this->_port = std::atoi(port.c_str());
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}
bool	env::set_host() {
	try
	{
		char hostname[1000];
		if (-1 == gethostname(hostname,1000))
			throw std::bad_exception();
		this->_hostname = hostname;
		// std::cout << hostname << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}
bool	env::set_limit() {
	try
	{
		struct rlimit	rlp;
		if (-1 == getrlimit(RLIMIT_NOFILE, &rlp))
			throw std::bad_exception();
		this->_maxfd = rlp.rlim_cur;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}

bool env::set_env(std::string port, std::string password) {
	if (!set_port(port))
		return false;
	if (!set_limit())
		return false;
	if (!set_host())
		return false;
	if (!set_server())
		return false;
	this->_password = password;
	return true;
}
bool env::set_server() {
	try
	{
		int opt=1;
		socklen_t optlen=sizeof(opt);
		int			s;
		struct sockaddr_in	sin;
		struct protoent	*pe;
		
		pe = (struct protoent*)err_void(NULL, getprotobyname("tcp"), "getprotobyname");
		s = err_int(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto ), "socket");
		err_int( -1, setsockopt(s ,SOL_SOCKET, SO_REUSEADDR, &opt, optlen), "sockopt");//| SO_REUSEADDR
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = INADDR_ANY;
		sin.sin_port = htons(this->_port);
		err_int(-1, bind(s, (struct sockaddr*)&sin, sizeof(sin)), "bind");
		err_int(-1, listen(s, 42), "listen");
		this->connections.push_back(new term_reader(FD_IO, STDIN_FILENO));
		this->connections.push_back(new server(FD_SERV, s));
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return false;
	}
	return true;
}
std::string	env::get_hostname() const{
	return this->_hostname;
}
std::string	env::get_pass() const{
	return this->_password;
}
int			env::get_port() const{
	return this->_port;
}
std::string	env::get_date() const {
	return this->_date;
}
std::vector<connection*>&	env::get_connections(){
	return this->connections;
}
std::vector<channel*>&		env::get_channels(){
	return this->channels;
}
void	env::init_fd()
{
	std::vector<connection*>::const_iterator start = this->connections.cbegin();
	std::vector<connection*>::const_iterator end = this->connections.cend();

	int	i;

	i = 0;
	this->_max = 0;
	FD_ZERO(&this->_fd_read);
	FD_ZERO(&this->_fd_write);
	// FD_SET(STDIN_FILENO, &e->fd_read);
	while (start != end)
	{
		FD_SET((*start)->get_fd(), &this->_fd_read);
		if ((*start)->get_bufwrite().length() > 0)
		{
			FD_SET((*start)->get_fd(), &this->_fd_write);
		}
		i++;
		this->_max = MAX(this->_max, (*start)->get_fd());
	// std::cout << (*start)->get_fd() << "init read\n";
		start++;
	}
}
void	env::do_select(){
	this->_select = select(this->_max + 1, &this->_fd_read, &this->_fd_write, NULL, NULL);
}
void	env::check_fd()
{
	std::vector<connection*>::const_iterator start = this->connections.cbegin();
	std::vector<connection*>::const_iterator end = this->connections.cend();
	int i = 0;
	int max = this->connections.size() - 1;
	while (this->connections.size() > i && (this->_select > 0))
	{
		if (this->connections[i] && FD_ISSET(this->connections[i]->get_fd(), &this->_fd_read))
		{
			if (!this->connections[i]->read(*this))
			{
				delete this->connections[i];
				this->connections[i] = 0;
			}

		}
		if (this->connections[i] && FD_ISSET(this->connections[i]->get_fd(), &this->_fd_write))
			this->connections[i]->write();
		if (this->connections[i] && FD_ISSET(this->connections[i]->get_fd(), &this->_fd_read) )
			this->_select--;
		else if (this->connections[i] && FD_ISSET(this->connections[i]->get_fd(), &this->_fd_write))
			this->_select--;
		if (this->connections[i])
			i++;
		else
			this->connections.erase(this->connections.begin()+i);
		max--;
	}
}

std::vector<client*> env::get_clients() {
	std::vector<client*> clients;

	for (auto &connection : this->connections) {
		if (connection->get_type() == FD_CLIENT) {
			if (client* client_ptr = dynamic_cast<client*>(connection)) {
				clients.push_back(client_ptr);
			}
		}
	}
	return (clients);
}

client	*env::search_client_nick( std::string nick_name)
{
	std::vector<client*> clients = get_clients();

	for (auto *client: clients) {
		if (client->is_registered() && client->get_nick() == nick_name)
			return (client);
	}
	return (NULL);
}

bool	env::nick_available( std::string nick_name)
{
	std::vector<client*> clients = get_clients();

	for (auto *temp_client: clients) {
		if (temp_client->is_registered() && temp_client->get_nick() == nick_name)
			return (false);
	}
	return (true);
}

bool	env::channel_exists( std::string name) {
	return (search_channel( name) != NULL);
}

channel *env::search_channel( std::string name) {
	for (auto *channel: (this->channels)) {
		if (channel->get_name() == name)
			return (channel);
	}
	return (NULL);
}

channel	*env::new_channel(std::string name, client *creator, std::string key) {
	if (channel_exists( name))
		throw(client_exception(messages::Client::ERR_UNAVAILRESOURCE, {name}));
	channel *new_channel = new channel(name);
	new_channel->add_client(creator);
	new_channel->add_operator(creator->get_nick());
	new_channel->set_key(key);
	this->channels.push_back(new_channel);
	return (new_channel);
}

// TODO: alot (check for key for example)
// change behavior when user is already in channel
channel *env::join_channel(std::string name, client *client, std::string key) {
	if (!channel_exists( name))
		throw(client_exception(messages::Client::ERR_NOSUCHCHANNEL, {name}));

	for (auto *channel: this->channels) {
		if (channel->get_name() == name) {
			if (!channel->check_key(key) && !channel->user_is_invited(client->get_nick()))
				throw(client_exception(messages::Client::ERR_BADCHANNELKEY, {name}));
			if (channel->user_in_channel(client->get_nick()))
				throw(client_exception(messages::Client::ERR_UNAVAILRESOURCE, {channel->get_name()}));
			if (channel->is_full())
				throw(client_exception(messages::Client::ERR_CHANNELISFULL, {channel->get_name()}));
			if (channel->get_invonly() && !channel->user_is_invited(client->get_nick()))
				throw(client_exception(messages::Client::ERR_INVITEONLYCHAN, {channel->get_name()}));
			channel->add_client(client);
			if (channel->user_is_invited(client->get_nick()))
				channel->remove_invite(client->get_nick());
			return (channel);
		}
	}
	return (NULL);
}
