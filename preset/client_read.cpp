#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "bircd.h"
#include "../other/signal.hpp"

static bool commands(t_env *e, int cs){
	t_fd client = e->fds[cs];
	if (client.buf_read.length() > 6 && !e->fds[cs].buf_read.compare(0, 6, "LOGIN "))
	{
		std::string temp(client.buf_read.substr(6,client.buf_read.size()));
		temp.pop_back();
		client.user->set_fullname(temp);
		client.user->set_nickname(temp);
		return false;
	}
	return true;
}
// static const char * recv_string(int cs)
// {
// 	std::string	buf;
// }
void	client_read(t_env *e, int cs)
{
	int		i;
	int		r;
	char	buf_read[BUF_SIZE + 1];

	r = recv(cs, buf_read, BUF_SIZE, 0);
	if (r <= 0)
	{
	  close(cs);
	  clean_client(&e->fds[cs]);
	  printf("client #%d gone away\n", cs);
	  return;
	}
	buf_read[r] = '\0';
	e->fds[cs].buf_read = buf_read;
	if (commands(e,cs))
	{
	  i = 0;
	  while (i < e->maxfd)
		{
		if ((e->fds[i].type == FD_CLIENT) && (i != cs))
		{
			std::string message = e->fds[cs].user->massege(e->fds[cs].buf_read);
			std::cout <<"massage> " <<message << "size>" << message.size()<< std::endl;
			send(i, message.c_str(),message.size() , 0);
		}
		i++;
		}
	}
}
