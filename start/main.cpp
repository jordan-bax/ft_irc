#include "../classes/env.hpp"
#include "../other/signal.hpp"

int main(int argc, char const *argv[])
{
	env e;

	if(!e.set_env(argv[1]))
		return 1;
	init_signalHandling();
	std::cout << "server is now listening to port "<< e.get_port() << " and hostname "<< e.get_hostname()<< std::endl;
	while (g_saveQuit)
	{
		e.init_fd();
		e.do_select();
		e.check_fd();
	}
	connection::clear(e.get_connections());
	return 0;
}
