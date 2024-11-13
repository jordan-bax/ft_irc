
#include "bircd.h"

// void	check_fd(t_env *e)
// {
// 	int	i;

// 	i = 0;
// 	while ((i < e->maxfd) && (e->r > 0))
// 	{
// 		if (FD_ISSET(i, &e->fd_read))
// 			e->fds[i].fct_read(e, i);
// 		if (FD_ISSET(i, &e->fd_write))
// 			e->fds[i].fct_write(e, i);
// 		if (FD_ISSET(i, &e->fd_read) || FD_ISSET(i, &e->fd_write))
// 			e->r--;
// 		i++;
// 	}
// }
void	check_fd(t_env *e)
{
	std::vector<connection*>::const_iterator start = e->connections.cbegin();
	std::vector<connection*>::const_iterator end = e->connections.cend();
	int i = 0;
	int max = e->connections.size() - 1;
	while (max >= 0 &&e->connections.size() > max && (e->r > 0))
	{
		// std::cout << e->connections[max]->get_fd() << " read\n";
		std::cout << e->connections.size() <<" > "<< max << " read\n";
		if (e->connections.size() > max && FD_ISSET(e->connections[max]->get_fd(), &e->fd_read))
		{
			e->connections[max]->read(e);
			std::cout << e->connections.size() <<" > "<< max << " read\n";
			// if (e->connections.size() < max)
			// {
			// 	max = e->connections.size();
			// 	if 
			// }
		}
		if (e->connections.size() > max && FD_ISSET(e->connections[max]->get_fd(), &e->fd_write))
			e->connections[max]->write();
		// if (e->connections.size() > max && FD_ISSET(e->connections[max]->get_fd(), &e->fd_read) || FD_ISSET(e->connections[max]->get_fd(), &e->fd_write))
		// 	e->r--;
		i++;
		max--;
	}
}
