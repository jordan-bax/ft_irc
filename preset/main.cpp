
// #include "bircd.h"
// #include "unistd.h"

// int	main(int ac, char **av)
// {
//   t_env	e;

//   init_env(&e);// get limit
//   get_opt(&e, ac, av); // check arg number and get port
//   srv_create(&e, e.port); //make server
//   main_loop(&e);// main loop

//   return (0);
// }

#include "../classes/env.hpp"
#include "../other/signal.hpp"

int main(int argc, char const *argv[])
{
	env e;

	if(!e.set_env(argv[1]))
		return 1;
	// std::cout << "test\n";
	init_signalHandling();
	std::cout << "server is now listening to port "<< e.get_port() << " and host "<< e.get_hostname()<< std::endl;
	while (g_saveQuit)
	{
		e.init_fd();
		e.do_select();
		e.check_fd();
	}
	connection::clear(e.get_connections());
	return 0;
}