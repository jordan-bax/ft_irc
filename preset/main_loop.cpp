#include "../classes/connection.hpp"
#include "bircd.h"
#include "../other/signal.hpp"
void	main_loop(t_env *e)
{
	init_signalHandling();
	std::cout << "server is now listening to port "<< e->port<< std::endl;
	while (g_saveQuit)
	{
		init_fd(e);
		do_select(e);
		check_fd(e);
	}
	connection::clear(e->connections);
	channel::clear(e->channels);
	
	// server_close(e);
}
