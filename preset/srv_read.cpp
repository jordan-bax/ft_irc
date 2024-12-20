#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "bircd.h"
#include "../other/signal.hpp"



// static bool commands(t_env *e, int cs){
// 	t_fd client = e->fds[cs];
// 	if (client.buf_read.length() > 4 && !e->fds[cs].buf_read.compare(0,4, "ALL "))
// 	{
// 		// std::string temp(client.buf_read+4);
// 		// temp.pop_back();
		
// 		return true;
// 	}
// 	return false;
// }

// void	srv_read(t_env *e, int cs)
// {
// 	int	r;
// 	int	i;
// 	std::string buf;
// 	std::getline(std::cin, buf, '\n');
// 	e->fds[cs].buf_read = buf;
// 	buf.append("\n");
// 	e->fds[cs].buf_read[buf.size()] = '\0';
// 	if (commands(e,cs))
// 	{
// 		i = 0;
// 		while (i < e->maxfd)
// 		{
// 			if ((e->fds[i].type == FD_CLIENT) && (i != cs))
// 			{
// 				// std::string message = e->fds[cs].user->massege(e->fds[cs].buf_read);
// 				// std::cout <<"massage> " <<message << "size>" << message.size()<< std::endl;
// 				send(i, buf.c_str(),buf.size() , 0);
// 			}
// 			i++;
// 		}
// 	}
// }