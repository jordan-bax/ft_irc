#include "../classes/env.hpp"
#include "../other/signal.hpp"
#include "../other/error_log.hpp"
#include "../classes/connection.hpp"
#include "../mycolor.hpp"

int main(int argc, char const *argv[])
{
	env e;

	
	if (argc != 3 && argc != 2)
		return 1;	
	if(argc == 3 && !e.set_env(argv[1], argv[2]))
		return 1;
	if(argc == 2 && !e.set_env(argv[1], "password"))
		return 1;
	init_signalHandling();
	log("hoi");
	std::cout << FG_GREEN"server is now listening to port "<< e.get_port() << " pass "<< e.get_pass()
		<<" and host "<< e.get_hostname() << "\nCreated on date " << e.get_date() \
		<< "with a max number of " << FD_SETSIZE<< FG_DEFAULT<<  std::endl;
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