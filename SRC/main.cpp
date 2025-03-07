#include "../HEADER/env.hpp"
#include "../HEADER/signal.hpp"
#include "../HEADER/error_log.hpp"
#include "../HEADER/connection.hpp"
#include "../HEADER/mycolor.hpp"



int main(int argc, char const *argv[])
{
	env e;

	on_exit([](int status, void *arg) {
		connection::clear((static_cast<env*>(arg))->get_connections());
		channel::clear((static_cast<env*>(arg))->get_channels());
		(static_cast<env*>(arg))->~env();
		log("exit");
	}, &e);
	if (argc != 3 && argc != 2)
		return 2;	
	if(argc == 3 && !e.set_env(argv[1], argv[2]))
		return 2;
	if(argc == 2 && !e.set_env(argv[1], "password"))
		return 2;
	init_signalHandling();
	log("hoi");
	std::cout << FG_GREEN"server is now listening to port "<< e.get_port() << " pass "<< e.get_pass()
		<<" and host "<< e.get_hostname() << "\nCreated on date " << e.get_date() \
		<< " with a max number of " << FD_SETSIZE<< FG_DEFAULT<<  std::endl;
	while (g_saveQuit)
	{
		e.init_fd();
		e.do_select();
		e.check_fd();
	}
	connection::clear(e.get_connections());
	channel::clear(e.get_channels());
	return 0;
}