
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
	while (e->connections.size() > i && (e->r > 0))
	{
		if (e->connections[i] && FD_ISSET(e->connections[i]->get_fd(), &e->fd_read))
		{
			if (!e->connections[i]->read(e))
			{
				delete e->connections[i];
				e->connections[i] = 0;
			}

		}
		if (e->connections[i] && FD_ISSET(e->connections[i]->get_fd(), &e->fd_write))
			e->connections[i]->write();
		if (e->connections[i] && FD_ISSET(e->connections[i]->get_fd(), &e->fd_read) )
			e->r--;
		else if (e->connections[i] && FD_ISSET(e->connections[i]->get_fd(), &e->fd_write))
			e->r--;
		if (e->connections[i])
			i++;
		else
			e->connections.erase(e->connections.begin()+i);
		max--;
	}
	// i = 0;
	// std::find(start,end, 0);
	// while (e->connections.size() < i)
	// {

	// }
}
