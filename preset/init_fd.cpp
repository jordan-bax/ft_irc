#include <unistd.h>

#include <string.h>
# include <sys/select.h>
#include "bircd.h"

// void	init_fd(t_env *e)
// {
// 	int	i;

// 	i = 0;
// 	e->max = 0;
// 	FD_ZERO(&e->fd_read);
// 	FD_ZERO(&e->fd_write);
// 	FD_SET(STDIN_FILENO, &e->fd_read);
// 	while (i < e->maxfd)
// 	{
// 		if (e->fds[i].type != FD_FREE)
// 		{
// 			FD_SET(i, &e->fd_read);
// 			if (e->fds[i].buf_write.length() > 0)
// 			{
// 				FD_SET(i, &e->fd_write);
// 			}
// 			e->max = MAX(e->max, i);
// 		}
// 		i++;
// 	}
// }

void	init_fd(t_env *e)
{
	std::vector<connection*>::const_iterator start = e->connections.cbegin();
	std::vector<connection*>::const_iterator end = e->connections.cend();

	int	i;

	i = 0;
	e->max = 0;
	FD_ZERO(&e->fd_read);
	FD_ZERO(&e->fd_write);
	// FD_SET(STDIN_FILENO, &e->fd_read);
	while (start != end)
	{
		FD_SET((*start)->get_fd(), &e->fd_read);
		if ((*start)->get_bufwrite().length() > 0)
		{
			FD_SET((*start)->get_fd(), &e->fd_write);
		}
		i++;
		e->max = MAX(e->max, (*start)->get_fd());
	// std::cout << (*start)->get_fd() << "init read\n";
		start++;
	}
}
